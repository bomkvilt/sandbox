import asyncio
import concurrent.futures
import multiprocessing
import multiprocessing.managers
import threading
from collections.abc import Callable, Coroutine
from typing import Any, Final, Self, Unpack, final, override

from .._actor import Actor, ActorBackendProtocol, ActorMethod
from .._scoped_object import ExcTuple, ScopedObject


@final
class _DeviceMessage[A: Actor, **P, R]:
    """ The class a method binding that would be called by an `remote` actor.
    """
    __slots__: Final = ("__method", "__kwargs", "__args", )

    def __init__(self, method: ActorMethod[A, P, R], *args: P.args, **kwargs: P.kwargs) -> None:
        self.__method: Final = method
        self.__kwargs: Final = kwargs
        self.__args: Final = args

    def __call__(self, actor: A) -> Coroutine[Any, Any, R]:
        return self.__method.__func__(actor, *self.__args, **self.__kwargs)


@final
class _SubprocessActorFactory[A: Actor, **AP]:
    """ The class is a factory that binds typing info and allows to create a new actor instance.
    """
    __slots__: Final = ("__actor_factory", "__actor_kwargs", "__actor_args", )

    def __init__(self, actor_factory: Callable[AP, A], *args: AP.args, **kwargs: AP.kwargs) -> None:
        self.__actor_factory: Final = actor_factory
        self.__actor_kwargs: Final = kwargs
        self.__actor_args: Final = args

    def __call__(self) -> A:
        return self.__actor_factory(*self.__actor_args, **self.__actor_kwargs)


@final
class _SubprocessActorWrapper[A: Actor, **AP]:
    """ A singlethon class that represents a subprocess actor class.
        NOTE: the actor must be terminated by a `.terminate()` method
    """
    def __new__(cls, *args, **kwargs):
        if not hasattr(cls, "__instance__"):
            cls.__instance__ = super().__new__(cls)
        return cls.__instance__

    def __init__(self, factory: _SubprocessActorFactory[A, AP] | None = None) -> None:
        if factory is not None:
            self.__actor: Final = factory()
            self.__loop: Final = asyncio.new_event_loop()
            self.__loop.run_until_complete(self.__actor.__aenter__())

    @classmethod
    def terminate(cls) -> None:
        assert hasattr(cls, "__instance__"), "The actor process is already terminated."
        wrapper: Final = _SubprocessActorWrapper()
        wrapper.__loop.run_until_complete(wrapper.__actor.__aexit__(None, None, None))
        delattr(cls, "__instance__")

    @classmethod
    def handle[**P, R](cls, msg: _DeviceMessage[A, P, R], canceled: threading.Event, /) -> R:
        async def cancel(task: asyncio.Task[Any]) -> None:
            # NOTE: we need to propagate cancelattion tokens betwean two async loops
            await asyncio.to_thread(canceled.wait)
            task.cancel()

        async def inner() -> R:
            coro: Final = msg(wrapper.__actor)  # NOTE: see below the method
            task: Final = wrapper.__loop.create_task(coro)
            cancel_task: Final = wrapper.__loop.create_task(cancel(task))
            try:
                return await task
            finally:
                cancel_task.cancel()

        wrapper: Final = _SubprocessActorWrapper()
        return wrapper.__loop.run_until_complete(inner())


@final
class SubprocessBackend[A: Actor, **AP](ScopedObject, ActorBackendProtocol):
    def __init__(self, actor_factory: Callable[AP, A], *args: AP.args, **kwargs: AP.kwargs) -> None:
        super().__init__()
        self.__process_pool: Final = concurrent.futures.ProcessPoolExecutor(
            # NOTE: I don't know what context should be used here
            # - fork contexts allow to use the code within Jupter Notebooks (and works faster)
            # - spawn contexts should create new environments
            mp_context=multiprocessing.get_context("fork"),

            # NOTE: we create an actor singletone in the new subprocess === process pool
            initializer=_SubprocessActorWrapper,
            initargs=(_SubprocessActorFactory(actor_factory, *args, **kwargs), ),
            max_workers=1,  # pool == actor
        )
        """ The process pool allows to use high level asyncio primitives.
        """

        self.__manager: Final = multiprocessing.Manager()
        """ The manager allows to share syncronisation (cancelation) primitives betwean processes.
        """

    @override
    async def __aenter__(self: Self) -> Self:
        await super().__aenter__()
        await self._enter_sync(self.__process_pool)
        await self._enter_sync(self.__manager)
        return self

    @override
    async def __aexit__(self, *exc: Unpack[ExcTuple]) -> None | bool:
        loop: Final = asyncio.get_event_loop()
        await loop.run_in_executor(self.__process_pool, _SubprocessActorWrapper.terminate)
        await super().__aexit__(*exc)

    async def send[**P, R](self, method: ActorMethod[A, P, R], *args: P.args, **kwargs: P.kwargs) -> R:
        canceled: Final = self.__manager.Event()
        try:
            loop: Final = asyncio.get_event_loop()
            msg: Final = _DeviceMessage(method, *args, **kwargs)
            return await loop.run_in_executor(self.__process_pool, _SubprocessActorWrapper.handle, msg, canceled)
        except BaseException:
            canceled.set()
            raise
