import dataclasses
import logging
import pathlib
from typing import Final, Literal, final

import pydantic


_TEMPLATE: Final = r"""
{% set os, arch, compiler, compiler_version = profile_name.split('-') %}

{% set clangxx = "<CLANGXX>" %}
{% set clang = "<CLANG>" %}

[settings]
os={{os}}
arch={{arch}}
compiler={{compiler}}
compiler.cppstd=23
compiler.version={{compiler_version}}
compiler.libcxx=libc++

build_type=Release

[conf]
# NOTE: single line
tools.build:compiler_executables={'c': '{{clang}}', 'cpp': '{{clangxx}}' }
"""


def _check_file_path(path: pathlib.Path) -> None:
    assert path.is_absolute(), f"all paths must be absolute: '{path}'"
    assert path.is_file(), f"the path is not a file: '{path}'"


_STRONG_MODEL_CONFIG: Final = pydantic.ConfigDict(
    validate_assignment=True,
    extra="forbid",
)


# TODO: move to a separate module with a profile builder base class
@final
@dataclasses.dataclass(kw_only=True, slots=True)
class ProfileBuilderResults:
    profile_text: str
    profile_name: str


@final
class ClangProfileBuilderParams(pydantic.BaseModel, **_STRONG_MODEL_CONFIG):
    arch: Literal["x86_64"]
    os: Literal["Linux"]

    compiler: Literal["clang"] = "clang"
    compiler_version: str = "17"

    check_paths: bool = True
    clang_path: pathlib.Path
    clangxx_path: pathlib.Path


def _validate_params(params: ClangProfileBuilderParams) -> None:
    if params.check_paths:
        _check_file_path(params.clangxx_path)
        _check_file_path(params.clang_path)


# TODO: split on a base class and implementation
@final
class ClangProfileBuilder:
    def __init__(self, params: ClangProfileBuilderParams) -> None:
        _validate_params(params)
        self.__params: Final = params.model_copy(deep=True)

    def build_profile(self) -> ProfileBuilderResults:
        return ProfileBuilderResults(
            profile_name=self.__render_profile_name(),
            profile_text=self.__render_profile(),
        )

    def __render_profile_name(self) -> str:
        return "-".join(
            [
                self.__params.os,
                self.__params.arch,
                self.__params.compiler,
                self.__params.compiler_version,
            ]
        )

    def __render_profile(self) -> str:
        # fmt: off
        return (
            _TEMPLATE
            .replace("<CLANGXX>", str(self.__params.clangxx_path))
            .replace("<CLANG>", str(self.__params.clang_path))
        )
        # fmt: on
