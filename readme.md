## How to build

The project uses a buzzle build system and conan package manager.
To configure them all:

* Create a new python (3.12+) virtual environment and install all required packages:
  ```sh
  pip install -r requirements.txt
  ```

* Install bazelisk:
  (a) https://bazel.build/start/cpp
  (b) https://github.com/bazelbuild/bazelisk/blob/master/README.md

* Setup a build environment:
  ```sh
  python ./tools/setup/setup.py
  ```

* Refresh `compile_commands.json`
  ```sh
  bazelisk build @hedron_compile_commands//:refresh_all
  ```

* Refresh `rust-project.json`
  ```sh
  bazelisk run @rules_rust//tools/rust_analyzer:gen_rust_project
  ```

* Apply auto formatters
  ```sh
  # rust
  bazelisk run @rules_rust//:rustfmt
  ```


## useful links

- clang++ standard versions support statuses
  https://libcxx.llvm.org/#c-dialect-support


## notes

- how to install a newmost python: <p>
  NOTE: Just a usefull information. <p>
  NOTE: The Python interpreter will be used in a local venv.
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

- how to update pip package versions:
  ```sh
  # update the packages themselves
  pip list --outdated --format=json | jq '.[].name' | xargs -n1 pip install --upgrade

  # update requirements.txt
  pip freeze > requirements.txt
  ```

- how to install a newmost clang++: <p>
  NOTE: Just a usefull information. <p>
  NOTE: Currently, the project uses Bazel build system with hermetic toolchains.
  ```sh
  # https://ubuntuhandbook.org/index.php/2023/09/how-to-install-clang-17-or-16-in-ubuntu-22-04-20-04/
  wget https://apt.llvm.org/llvm.sh
  chmod u+x llvm.sh
  sudo ./llvm.sh 17 all
  ```

- how to install rust: <p>
  NOTE: Even if Bazel loads the sealed toolchain, it doesn't expose rustfmt to the sysroot.
    Therefore, it's necessary to add an external formatter binary to enable formatting in the IDE.
  ```sh
  # https://doc.rust-lang.org/book/ch01-01-installation.html
  curl --proto '=https' --tlsv1.2 https://sh.rustup.rs -sSf | sh
  ```

- how to integrate CMake with clangd:
  https://github.com/SpaceIm/vscode-cpp-cmake-conan-template/blob/conan-v2-ninja-no-presets/.vscode/settings.json

- how to integrate bazel with clangd:
  https://github.com/hedronvision/bazel-compile-commands-extractor
