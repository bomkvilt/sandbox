import sys
from typing import Final

import conan
import conan.tools.cmake

print(f"using typthon version: {sys.version}")


class HelloConan(conan.ConanFile):
    settings: Final = ("os", "compiler", "build_type", "arch")

    # NOTE: run "conan-01: install" vscode task
    requires: Final = (
        "benchmark/1.8.3",
        "gtest/1.14.0",
    )

    generators: Final = (
        "CMakeToolchain",
        "CMakeDeps",
    )

    def cmake_layout(self) -> None:
        conan.tools.cmake.cmake_layout(self)

    def build(self) -> None:
        cmake: Final = conan.tools.cmake.CMake(self)
        cmake.configure()
        cmake.build()
