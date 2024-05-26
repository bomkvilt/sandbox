import asyncio
import contextlib
import logging
import os
import types
from typing import Final, Protocol, Self, TypeAlias, Unpack, final, override, runtime_checkable

ExcTuple: TypeAlias = tuple[type[BaseException] | None, BaseException | None, types.TracebackType | None]
"""
```py
def __exit__(self, *exc: Unpack[ExcTuple]) -> bool | None:
    super().__exit__(*exc)
    et, ev, tb = exc
```
"""

_DOUBLE_ENTER_ERROR: Final = "double enter is prohibited"
_DOUBLE_EXIT_ERROR: Final = "double exit is prohibited"
_NOT_ENTERED_ERROR: Final = "scoped object has not been entered"
_NOT_EXITED_ERROR: Final = "scoped object has not been exited"


@runtime_checkable
class _AsyncContextManagerProtocol[T](contextlib.AbstractAsyncContextManager[T], Protocol):
    """ Hack that allows to deduce an entity that would be returned. """
    async def __aenter__(self: Self) -> T: ...
    async def __aexit__(self, et, ev, tb, /) -> bool | None:  ...


@runtime_checkable
class _ContextManagerProtocol[T](contextlib.AbstractContextManager[T], Protocol):
    """ Hack that allows to deduce an entity that would be returned. """
    def __enter__(self: Self) -> T: ...
    def __exit__(self, et, ev, tb, /) -> bool | None:  ...


class ScopedObjectError(RuntimeError):
    pass


class ScopedObject(contextlib.AbstractAsyncContextManager):
    """ Base class that allows to create nested RAII entities.
    """

    def __init__(self) -> None:
        self.__entered: Final = asyncio.Event()
        self.__exited: Final = asyncio.Event()

        self.__stack: Final = contextlib.AsyncExitStack()

        self.__balance = 0
        """ enter-exit ballance: `count of enters` - `count of exits`.
        """

    def __del__(self) -> None:
        # NOTE: Users can miss to call parent's __enter__ or __exit__ methods.

        if not self.__entered.is_set():
            logging.critical(f"{_NOT_ENTERED_ERROR}: {type(self)}")
            os.abort()  # exceptions will be ignored

        if not self.__exited.is_set() or self.__balance != 0:
            logging.critical(f"{_NOT_EXITED_ERROR}: {type(self)}, ballance: {self.__balance}")
            os.abort()  # exceptions will be ignored

    @override
    async def __aenter__(self: Self) -> Self:
        if self.__entered.is_set() and not self._is_stateless():
            raise ScopedObjectError(_DOUBLE_ENTER_ERROR)

        if not self.__entered.is_set():
            # NOTE: can raise
            await self.__stack.__aenter__()

        # NOTE: noexcept
        self.__entered.set()
        self.__balance += 1
        return self

    @override
    async def __aexit__(self, *exc: Unpack[ExcTuple]) -> bool | None:
        if not self.__entered.is_set():
            raise ScopedObjectError(_NOT_ENTERED_ERROR)

        if self.__exited.is_set() and not self._is_stateless():
            raise ScopedObjectError(_DOUBLE_EXIT_ERROR)

        # NOTE: the stack below can rise exceptions
        self.__balance -= 1
        self.__exited.set()

        return await self.__stack.__aexit__(*exc)

    def _is_stateless(self) -> bool:
        """ Flag showing that the object can be entered & exited multple times. """
        return False

    # NOTE: do not add `asyncio.TaskGroup` into the class!
    # Probably, you do not want 20 nested ExceptionGroups in a stack trace

    @final
    async def _enter_async[T](self, ctx: _AsyncContextManagerProtocol[T]) -> T:
        """ Move the context's ownership to the scoped object. """
        return await self.__stack.enter_async_context(ctx)

    @final
    async def _enter_sync[T](self, ctx: _ContextManagerProtocol[T]) -> T:
        """ Move the context's ownership to the scoped object. """
        return self.__stack.enter_context(ctx)
