import dataclasses
import json
import logging
import pathlib
import subprocess
from typing import Final, final

import _internal


_CONAN_PROFILES_ROOT: Final = "build/conan-profiles"

# NOTE: I am not shure that the following target names are stable
# If so, you can use the dummy target which is next to the script
_TOOLCHAIN_TARGET: Final = "@@rules_cc+//cc:current_cc_toolchain"


def _check_directory(path: pathlib.Path, *, optional: bool = False) -> None:
    if optional and not path.exists():
        return
    assert path.is_dir(), f"the path is not a directory: '{path}'"


def _query_bazelisk_info(key: str) -> str:
    proc: Final = subprocess.run(
        ["bazelisk", "info", key],
        check=True,
        text=True,
        capture_output=True,
    )
    return proc.stdout.strip()


def _find_output_root() -> pathlib.Path:
    path: Final = pathlib.Path(_query_bazelisk_info("output_base").strip())
    _check_directory(path)
    return path


def _find_workspace_root() -> pathlib.Path:
    path: Final = pathlib.Path(_query_bazelisk_info("workspace").strip())
    _check_directory(path)
    return path


@final
@dataclasses.dataclass(slots=True, kw_only=True)
class _CompilerPaths:
    clang: pathlib.Path
    clangxx: pathlib.Path


def _find_toolchain() -> _CompilerPaths:
    proc: Final = subprocess.run(
        [
            *["bazelisk", "cquery", _TOOLCHAIN_TARGET],
            "--output=starlark",
            "--starlark:expr=[x.path for x in target.files.to_list()]",
        ],
        check=True,
        text=True,
        stdout=subprocess.PIPE,
    )

    paths: Final = dict[str, pathlib.Path]()
    for line in proc.stdout.strip().splitlines():
        for path in json.loads(line):
            path = pathlib.Path(path)
            if path.name not in ("clang", "clang++"):
                continue

            if path.name not in paths:
                paths[path.name] = path
                continue

            if paths[path.name] != path:
                raise ValueError(f"multiple paths for '{path.name}' found: '{paths[path.name]}' and '{path}'")

    return _CompilerPaths(
        clang=paths["clang"],
        clangxx=paths["clang++"],
    )


@final
class _SetupManager:
    def __init__(self) -> None:
        self.__workspace_root: Final = _find_workspace_root()
        logging.info(f"workspace root: {self.__workspace_root}")

        self.__output_root: Final = _find_output_root()
        logging.info(f"output root: {self.__workspace_root}")

        self.__profile_root: Final = self.__workspace_root / _CONAN_PROFILES_ROOT
        _check_directory(self.__profile_root, optional=True)

        if not self.__profile_root.exists():
            self.__profile_root.mkdir()

    def create_profile(self) -> pathlib.Path:
        tools: Final = _find_toolchain()

        builder_params: Final = _internal.ClangProfileBuilderParams(
            # toolchain
            # TODO: deduce from the toolchain
            compiler_version="19",
            arch="x86_64",
            os="Linux",
            # paths
            clangxx_path=(self.__output_root / tools.clangxx),
            clang_path=(self.__output_root / tools.clang),
        )

        builder: Final = _internal.ClangProfileBuilder(builder_params)
        profile: Final = builder.build_profile()
        profile_path: Final = self.__profile_root / profile.profile_name

        logging.info(f"writing profile to: {profile_path}")
        profile_path.write_text(profile.profile_text)

        return profile_path

    def setup_bazel(self) -> None:
        subprocess.run(
            ["bazelisk", "build", _TOOLCHAIN_TARGET],
            capture_output=False,
            check=True,
        )

    def run_conan(self, profile_path: pathlib.Path) -> None:
        subprocess.run(
            [
                *["conan", "install", "."],
                "--build=missing",
                f"--profile:build={profile_path}",
                f"--profile:host={profile_path}",
            ],
            capture_output=False,
            check=True,
        )


def main() -> None:
    logging.basicConfig(level=logging.INFO)

    setup: Final = _SetupManager()
    setup.setup_bazel()

    profile_path: Final = setup.create_profile()
    setup.run_conan(profile_path)


if __name__ == "__main__":
    main()
