import logging
import os
import pathlib
import subprocess
from typing import Final, final

import _internal


_LLVM_RELATIVE_ROOT: Final = "./external/toolchains_llvm~~llvm~llvm_toolchain_llvm/bin"
_CONAN_PROFILES_ROOT: Final = "./build/conan-profiles"


def _check_directory(path: pathlib.Path, *, missing: bool = False) -> None:
    if missing and not path.exists():
        return
    assert path.is_dir(), f"the path is not a directory: '{path}'"


def _deduce_buld_root() -> pathlib.Path:
    proc: Final = subprocess.run(["bazelisk", "info", "output_base"], check=True, text=True, capture_output=True)
    proc.check_returncode()

    path: Final = pathlib.Path(proc.stdout.strip())
    _check_directory(path)
    return path


@final
class _Setup:
    def __init__(self) -> None:
        self.__proj_root: Final = pathlib.Path(os.getcwd())
        _check_directory(self.__proj_root)

        self.__llvm_root: Final = _deduce_buld_root() / _LLVM_RELATIVE_ROOT
        _check_directory(self.__llvm_root, missing=True)

        self.__profile_root: Final = self.__proj_root / _CONAN_PROFILES_ROOT
        _check_directory(self.__profile_root, missing=True)

        if not self.__profile_root.exists():
            self.__profile_root.mkdir()

        logging.info(f"project root: {self.__proj_root}")
        logging.info(f"llvm root: {self.__llvm_root}")

    def create_profile(self) -> pathlib.Path:
        builder_params: Final = _internal.ClangProfileBuilderParams(
            # toolchain
            compiler_version="17",
            arch="x86_64",
            os="Linux",
            # paths
            clangxx_path=(self.__llvm_root / "clang++"),
            clang_path=(self.__llvm_root / "clang"),
        )

        builder: Final = _internal.ClangProfileBuilder(builder_params)
        profile: Final = builder.build_profile()
        profile_path: Final = self.__profile_root / profile.profile_name

        logging.info(f"writing profile to: {profile_path}")
        profile_path.write_text(profile.profile_text)

        return profile_path

    def setup_bazel(self) -> None:
        subprocess.run(
            ["bazelisk", "build", "tools/setup/_dummy"],
            capture_output=False,
            check=True,
        )

    def run_conan(self, profile_path: pathlib.Path) -> None:
        subprocess.run(
            ["conan", "install", ".", "--build=missing", f"--profile={profile_path}"],
            capture_output=False,
            check=True,
        )


def main() -> None:
    logging.basicConfig(level=logging.INFO)

    setup: Final = _Setup()
    setup.setup_bazel()

    profile_path: Final = setup.create_profile()
    setup.run_conan(profile_path)


if __name__ == "__main__":
    main()
