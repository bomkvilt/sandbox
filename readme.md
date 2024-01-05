##


## How to build

The project uses Conan package manager. Main tasks and settings are listed in a `./.vscode` derectory.

* clone all required submodules
  ```sh
  git submodule update
  ```

* Create all requred profile files for all required cmake kits (if the profiles are missing):
  * profiles are located in `~/.conan2/profiles/<profile name>` files

  * `default` profile can be created with the command
    ```sh
    conan profile detect --force
    ```

  * real profile example (`~/.conan2/profiles/Clang 17.0.6 x86_64-pc-linux-gnu`):
    ```ini
    [settings]
    arch=x86_64
    build_type=Release
    compiler=clang        # change to see possible variants; {gcc; clang; ...}
    compiler.version=17   # clang++17
    os=Linux
    ```

* Install a task runner extension (e.g. (Task Runner)[<https://marketplace.visualstudio.com/items?itemName=forbeslindesay.forbeslindesay-taskrunner>]).
  * run `conan-01: install` (will be called automatically on the next step)
  * run `conan-02: initialize`
