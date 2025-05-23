import sys
from typing import Final, final

import conan  # type: ignore
import conan.tools.google  # type: ignore


print(f"used python version: {sys.version}")


@final
class Conan(conan.ConanFile):
    settings: Final = ("os", "compiler", "build_type", "arch")

    def requirements(self) -> None:
        assert self.requires is not None
        self.requires("asio/1.30.2")
        self.requires("gtest/1.15.0")

    def build_requirements(self) -> None:
        pass

    def configure(self) -> None:
        pass

    def layout(self) -> None:
        self.folders.generators = "build/conan"
        conan.tools.google.bazel_layout(self)

    def generate(self) -> None:
        bazel_deps: Final = conan.tools.google.BazelDeps(self)
        bazel_deps.generate()
