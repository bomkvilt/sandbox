import asyncio
import concurrent.futures
import dataclasses
import multiprocessing
import multiprocessing.connection
import queue
from collections.abc import Callable
from types import TracebackType
from typing import Any, Awaitable, Final, Self, final

from asyncio_actor._actor import Actor, ActorBackend, ActorMethod
from asyncio_actor._scoped_object import ScopedObject


@final
class _DeviceMessage[A: Actor, **P, R]:
    def __init__(self, method: ActorMethod[A, P, R], *args: P.args, **kwargs: P.kwargs) -> None:
        self.method = method
        self.kwargs = kwargs
        self.args = args


@final
@dataclasses.dataclass(kw_only=True, slots=True)
class _HostMessage[A: Actor, **P, R]:
    future: concurrent.futures.Future[R]
    payload: _DeviceMessage[A, P, R]


@final
@dataclasses.dataclass(kw_only=True, slots=True)
class _DeviceResponse:
    result: Any | None = None
    exception: Exception | None = None

    def fill(self, future: concurrent.futures.Future[Any]) -> None:
        if self.exception is not None:
            future.set_exception(self.exception)
        else:
            future.set_result(self.result)


def _run_new_loop[**P, R](action: Callable[P, Awaitable[R]]) -> Callable[P, R]:
    def wraper(*args: P.args, **kwargs: P.kwargs) -> R:
        loop: Final = asyncio.new_event_loop()
        try:
            asyncio.set_event_loop(loop)
            return loop.run_until_complete(action(*args, **kwargs))
        finally:
            loop.close()
    return wraper


@final
class SubprocessBackend[A: Actor, **AP](ScopedObject, ActorBackend):
    def __init__(self, actor_factory: Callable[AP, A], *args: AP.args, **kwargs: AP.kwargs) -> None:
        super().__init__()
        self.__actor_factory: Final = actor_factory
        self.__actor_kwargs: Final = kwargs
        self.__actor_args: Final = args
        self.__queue: Final = queue.Queue[_HostMessage[Any, Any, Any] | None]()
        self.__done: Final = asyncio.Event()

    async def __aenter__(self: Self) -> Self:
        await super().__aenter__()
        self._create_subtask(asyncio.to_thread(self._run_collector))
        return self

    async def __aexit__(self, et: type[BaseException] | None, ev: BaseException | None, tb: TracebackType | None) -> None:
        await self.stop()
        await super().__aexit__(et, ev, tb)

    async def stop(self) -> None:
        self.__queue.put(None)

    async def send[**P, R](self, method: ActorMethod[A, P, R], *args: P.args, **kwargs: P.kwargs) -> R:
        task: Final = _HostMessage(
            future=concurrent.futures.Future(),
            payload=_DeviceMessage(method, *args, **kwargs),
        )
        self.__queue.put(task)
        return await asyncio.wrap_future(task.future)

    @_run_new_loop
    async def _run_subprocess(self, pipe: multiprocessing.connection.Connection) -> None:
        async with self.__actor_factory(*self.__actor_args, **self.__actor_kwargs) as actor:
            while (request := pipe.recv()) is not None:
                try:
                    assert isinstance(request, _DeviceMessage)
                    result = await request.method.__func__(actor, *request.args, **request.kwargs)
                    pipe.send(_DeviceResponse(result=result))
                except Exception as e:
                    pipe.send(_DeviceResponse(exception=e))
            pipe.send(None)

    def _run_collector(self) -> None:
        hst_pipe, dev_pipe = multiprocessing.Pipe()
        process: Final = multiprocessing.Process(target=self._run_subprocess, args=(dev_pipe, ))
        try:
            process.start()

            while (task := self.__queue.get()) is not None:
                hst_pipe.send(task.payload)
                response: Final = hst_pipe.recv()
                assert isinstance(response, _DeviceResponse)
                response.fill(task.future)

            hst_pipe.send(None)
            process.join()

        except BaseException:
            process.terminate()
            process.join()
            raise

        finally:
            self.__done.set()
