# c++20.modules

Useful links:
- https://vector-of-bool.github.io/2019/03/10/modules-1.html


This sketch demonstrates how to create a demposed c++20 module with:
- a private module segment;
- partition units;
- implementation units;
- standalone tests for public and private entities

using CMAKE.

The demo were written on CMake 3.28.1 and compiled using clang++-17.

The code contains explanatory commentaries.

## studied best tractices
* Module must be realised using interface and implementation units (like .hpp and .cpp before)
    ```cpp
    // module.cppm
    ...
    export module module_name;
    ...

    // module.cpp
    ...
    module module_name;
    ...
    ```

    ```cmake
    # CMakeLists.txt
    target_sources(
        <library_name>
        PUBLIC
            # NOTE: list of module files that expose `export module <>;` only
            FILE_SET public_modules TYPE CXX_MODULES FILES
            module.cppm
        PRIVATE
            # NOTE: implementation units are ordinary .cpp files with additional <...>@module_name mangling
            module.cpp
        )
    ```

* Module must be relatively small to reduce bild cache invalidation effects (?)

* Unit tests must be realised as implementation units that use the same module name
    ```cpp
    // module.cppm
    ...
    export module module_name;
    ...

    // ut/test.cpp
    ...
    module module_name;
    ...
    ```
