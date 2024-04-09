import copy
from collections.abc import Callable
from typing import Final, Self, final

from ._actor import Actor, ActorBackend, ActorBackendFactory, AsyncioActorRef
from .backends import DummyBackend


@final
class ActorSpawner[**P, A: Actor]:
    def __init__(self, actor_factory: Callable[P, A]) -> None:
        self.__actor_factory: Final = actor_factory
        self.__backend_actory: ActorBackendFactory[A, P] = DummyBackend[A, P]

    def __call__(self, *args: P.args, **kwargs: P.kwargs) -> AsyncioActorRef[A]:
        backend: Final = self.__backend_actory(self.__actor_factory, *args, **kwargs)
        assert isinstance(backend, ActorBackend)
        return AsyncioActorRef[A](backend)

    def using_backend(self: Self, factory: ActorBackendFactory[A, P]) -> Self:
        patched: Final = copy.copy(self)
        patched.__backend_actory = factory
        return patched
