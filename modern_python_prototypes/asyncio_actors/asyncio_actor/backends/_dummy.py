import asyncio
from collections.abc import Callable
from typing import Final, Self, final

from .._actor import Actor, ActorBackendProtocol, ActorMethod
from .._scoped_object import ScopedObject


@final
class DummyBackend[A: Actor, **AP](ScopedObject, ActorBackendProtocol):
    def __init__(self, actor_factory: Callable[AP, A], *args: AP.args, **kwargs: AP.kwargs) -> None:
        super().__init__()
        self.__actor_factory: Final = lambda: actor_factory(*args, **kwargs)
        self.__lock: Final = asyncio.Lock()

        self.__actor: A | None = None

    async def __aenter__(self: Self) -> Self:
        await super().__aenter__()
        self.__actor = await self._enter_async(self.__actor_factory())
        return self

    async def send[**P, R](self, method: ActorMethod[A, P, R], *args: P.args, **kwargs: P.kwargs) -> R:
        async with self.__lock:  # actor cannot process messages in parallel
            assert self.__actor is not None, "the backend was not entered"
            return await method.__func__(self.__actor, *args, **kwargs)
