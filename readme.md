## How to build

> NOTE: the project uses CMake since bazzel does not support c++20 models from the box
> https://github.com/bazelbuild/bazel/issues/4005


The project uses Conan package manager. Main tasks and settings are listed in a `./.vscode` derectory.

* load all the required submodules
  ```sh
  git submodule update
  ```

* Create all requred profile files for all required cmake kits (if the profiles are missing):
  * profiles are located in `~/.conan2/profiles/<profile name>` files

  * `default` profile can be created with the command
    ```sh
    conan profile detect --force
    ```

  * profile example (`~/.conan2/profiles/Clang 17.0.6 x86_64-pc-linux-gnu`):
    ```ini
    [settings]
    arch=x86_64
    compiler=clang          # change to see possible variants; {gcc; clang; ...}
    compiler.version=17     # clang++17
    compiler.libcxx=libc++  # use clang's c++ std library (support of c++{20, 23} features)
    build_type=Release
    os=Linux

    [conf]
    # by default conan uses c++ compiler regardless on the `compiler=clang` line above
    # NOTE: single line expression
    tools.build:compiler_executables={'c': '/usr/bin/clang-17', 'cpp': '/usr/bin/clang++-17'}
    ```

* Install a task runner extension (e.g. (Task Runner)[<https://marketplace.visualstudio.com/items?itemName=forbeslindesay.forbeslindesay-taskrunner>]).
  * run `conan-01: install` (The step will build specfied conan packages)
  * run `configure` (The step will initialize all the cmake stuff and configure the project)
  * run `build`


## useful links
- clang++ standard versions support statuses
  https://libcxx.llvm.org/#c-dialect-support


## other notes

- how to install a newmost python
  ```sh
  # add repo and install required packages
  sudo apt update
  sudo apt install software-properties-common -y
  sudo add-apt-repository ppa:deadsnakes/ppa -y
  sudo apt install python3.12

  # check that python was installed
  python3.12 --version

  # install libs
  sudo apt install python3.12-dev
  sudo apt install python3.12-venv
  sudo apt install python3.12-distutils

  # update links
  sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.12 12
  ```

- how to install a newmost clang++
  ```sh
  # https://ubuntuhandbook.org/index.php/2023/09/how-to-install-clang-17-or-16-in-ubuntu-22-04-20-04/
  wget https://apt.llvm.org/llvm.sh
  chmod u+x llvm.sh
  sudo ./llvm.sh 17 all
  ```
