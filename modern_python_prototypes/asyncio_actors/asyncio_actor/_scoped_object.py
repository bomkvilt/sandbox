import asyncio
import contextlib
import logging
import os
import types
from collections.abc import Coroutine
from typing import Any, Final, Self

_DOUBLE_ENTER_ERROR: Final = "double enter is prohibited"
_DOUBLE_EXIT_ERROR: Final = "double exit is prohibited"
_NOT_ENTERED_ERROR: Final = "scoped object was not entered"
_NOT_EXITED_ERROR: Final = "scoped object was not exited"


class ScopedObjectError(RuntimeError):
    pass


class ScopedObject(contextlib.AbstractAsyncContextManager):
    """ Base class that allows to create nested RAII entities.
    """

    def __init__(self) -> None:
        self.__entered: Final = asyncio.Event()
        self.__exited: Final = asyncio.Event()
        self.__stack: Final = contextlib.AsyncExitStack()
        self.__tasks: asyncio.TaskGroup | None = None

        self.__balance = 0
        """ enter-exit ballance: `count of enters` - `count of exits`.
        """

    def __del__(self) -> None:
        # NOTE: user can miss to call parent's __enter__ or __exit__ methods.
        # The checks is an only way to ensure that the scope object was closed I see.

        if not self.__entered.is_set():
            logging.critical(f"{_NOT_ENTERED_ERROR}: {type(self)}")
            os.abort()  # exceptions does not work

        if not self.__exited.is_set() or self.__balance != 0:
            logging.critical(f"{_NOT_EXITED_ERROR}: {type(self)}, ballance: {self.__balance}")
            os.abort()  # exceptions does not work

    async def __aenter__(self: Self) -> Self:
        if self.__entered.is_set() and not self._is_stateless():
            raise ScopedObjectError(_DOUBLE_ENTER_ERROR)

        if not self.__entered.is_set():
            self.__tasks = await self.__stack.enter_async_context(asyncio.TaskGroup())
            self.__entered.set()  # the latest

        self.__balance += 1
        return self

    async def __aexit__(
        self, et: type[BaseException] | None, ev: BaseException | None, tb: types.TracebackType | None, /,
    ) -> None: 
        if not self.__entered.is_set():
            raise ScopedObjectError(_NOT_ENTERED_ERROR)

        if self.__exited.is_set() and not self._is_stateless():
            raise ScopedObjectError(_DOUBLE_EXIT_ERROR)

        self.__balance -= 1
        self.__exited.set()  # NOTE: the stack below can rise exceptions
        await self.__stack.__aexit__(et, ev, tb)

    def _is_stateless(self) -> bool:
        """ Flag showing that the object can be entered & exited multple times. """
        return False

    async def _enter_async[T](self, ctx: contextlib.AbstractAsyncContextManager[T]) -> T:
        """ Move the context's ownership to the scoped object. """
        return await self.__stack.enter_async_context(ctx)

    async def _enter_sync[T](self, ctx: contextlib.AbstractContextManager[T]) -> T:
        """ Move the context's ownership to the scoped object. """
        return self.__stack.enter_context(ctx)

    def _create_subtask[R](self, coro: Coroutine[Any, Any, R]) -> asyncio.Task[R]:
        """ Launch the asyncio task as a child task of the scoped object's task. """
        if self.__tasks is None:
            raise ScopedObjectError(_NOT_ENTERED_ERROR)
        return self.__tasks.create_task(coro)
