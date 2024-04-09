from __future__ import annotations

import abc
import contextlib
import typing
from collections.abc import Callable
from typing import Any, Concatenate, Coroutine, Final, Protocol, Self, final, runtime_checkable

from ._scoped_object import ScopedObject


class ActorMetaClass(abc.ABCMeta):
    def __new__(mcls, name: str, bases: tuple[type, ...], namespace: dict[str, Any], /, **kwargs: Any):
        cls = super().__new__(mcls, name, bases, namespace, **kwargs)
        for field_name, field_type in list(cls.__dict__.items()):
            if isinstance(field_type, ActorMethod):
                field_type._update_action(cls, field_name)
        return cls


@runtime_checkable
class ActorBackend[A: 'Actor'](contextlib.AbstractAsyncContextManager, Protocol):
    async def send[**P, R](self, method: ActorMethod[A, P, R], *args: P.args, **kwargs: P.kwargs) -> R:
        ...


@runtime_checkable
class ActorBackendFactory[A: 'Actor', **AP](Protocol):
    def __call__(self, actor_factory: Callable[AP, A], *args: AP.args, **kwargs: AP.kwargs) -> ActorBackend[A]:
        ...


_ATTRS_TO_COPY: Final = ("__module__", "__name__", "__qualname__", )
_ATTRS_TO_UNIQ: Final = ("__name__", "__qualname__", )


@final
class ActorMethod[S: 'Actor', **P, R]:
    def __init__(self, action: Callable[Concatenate[S, P], Coroutine[Any, Any, R]]) -> None:
        for attr in _ATTRS_TO_COPY:
            setattr(self, attr, getattr(action, attr))
        self.__action: Final = action

    def _update_action(self, cls: type[Actor], field_name: str) -> None:
        assert self.__action.__name__ == field_name, f"'{self.__action.__name__}' vs. '{field_name}'"
        for attr in _ATTRS_TO_UNIQ:
            setattr(self.__action, attr, getattr(self.__action, attr) + "_wrapped")
        setattr(cls, self.__action.__name__, self.__action)

    @property
    def __func__(self) -> Callable[Concatenate[S, P], Coroutine[Any, Any, R]]:
        return self.__action

    @typing.overload
    def __get__(self: Self, instance: None, owner: type | None = None, /) -> Self:
        ...

    @typing.overload
    def __get__(self: Self, instance: S, owner: type | None = None, /) -> Callable[P, Coroutine[Any, Any, R]]:
        ...

    def __get__(self, instance: S | None, owner: type | None = None, /):
        if instance is None:
            return self

        async def wrapped(*args: P.args, **kwargs: P.kwargs) -> R:
            return await self.__action(instance, *args, **kwargs)

        return wrapped


class Actor(ScopedObject, metaclass=ActorMetaClass):
    """ Asymmetric actor class.
    """


def actor_method():
    def decorator[S: Actor, **P, R](action: Callable[Concatenate[S, P], Coroutine[Any, Any, R]], /):
        method = ActorMethod(action)
        return method
    return decorator


@final
class ActorRef[A: Actor](ScopedObject):
    def __init__(self, backend: ActorBackend[A]) -> None:
        super().__init__()
        self.__backend = backend

    async def __aenter__(self: Self) -> Self:
        await super().__aenter__()
        await self._enter_async(self.__backend)
        return self

    async def task[**P, R](self, method: ActorMethod[A, P, R], *args: P.args, **kwargs: P.kwargs) -> R:
        return await self.__backend.send(method, *args, **kwargs)
