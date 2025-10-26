# Sandbox

## How to build

### 1. Create a new python (3.12+) virtual environment and install required packages

```sh
# python -m pip install virtualenv
# python -m virtualenv venv
# <and enter the venv>

pip install -r requirements.txt
```

### 2. Install bazelisk

(a) <https://github.com/bazelbuild/bazelisk/blob/master/README.md#installation>
(b) <https://bazel.build/start/cpp>

If you, suddenly, use Windows (out of WSL) — the installed binaries will not be visible untill reboot.

### 3. Setup C++ toolchain and environment

The script works for linux only as it brings a hermetic llvm toolchain using the llvm_toolchain rule,
  which is suddenly not supported for Windows platforms yet <https://github.com/bazel-contrib/toolchains_llvm/pull/342>

The main purpose of the step for now is to configure Conan dependencies.

```sh
python ./build/setup/setup.py
```

### 4. Apply auto formatters

```sh
# rust
bazelisk run @rules_rust//:rustfmt

# NOTE: `.bazelrc` file contains enforced style checks
```

### 5. Generate compile commands to enable IDE support

```sh
# c++: compile_commands.json -> vscode::clangd
bazelisk build @hedron_compile_commands//:refresh_all

# rust: rust-project.json -> vscode::rust-analyzer
bazelisk run @rules_rust//tools/rust_analyzer:gen_rust_project
```

### 6. Build or Test

```sh
# build
bazelisk build //rust/basics/00.hellow_world:hello_lib_test

# test
bazelisk test //rust/basics/00.hellow_world:hello_lib_test
```

## useful links

- clang++ standard versions support statuses:
  <https://libcxx.llvm.org/#c-dialect-support>

## notes

- recomended `user.bazelrc`:

  ```bazelrc
  build --disk_cache=~/.bazel_cache/build
  build --experimental_disk_cache_gc_max_size=40G
  build --experimental_disk_cache_gc_max_age=30d
  build --experimental_disk_cache_gc_idle_delay=5m

  build --repository_cache=~/.bazel_cache/repos
  ```

- how to install a new-most python:

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
  sudo update-alternatives --install /usr/bin/python python /usr/bin/python3.12 12
  ```

- how to update pip package versions:

  ```sh
  # update the packages themselves
  pip list --outdated --format=json | jq '.[].name' | xargs -n1 pip install --upgrade

  # update requirements.txt
  pip freeze > requirements.txt
  ```

- how to install a new-most clang++:

  For Linux platform the project uses Bazel build system with a hermetic toolchains by using of the llvm_toolchain
    bazel module.

  However for some other cases, LLVM can be insalled this way:

  ```sh
  # install
  wget https://apt.llvm.org/llvm.sh
  chmod u+x llvm.sh
  sudo ./llvm.sh 21 all

  # update links
  sudo update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-21 21
  ```

  In case modern python version (e.g. 3.13) apt_pkg package can be absent:

  ```sh
  sudo apt install --reinstall python3-apt
  ls -la /usr/lib/python3/dist-packages/apt_pkg* # /usr/lib/python3/dist-packages/apt_pkg.cpython-312-x86_64-linux-gnu.so
  sudo ln -s \
    /usr/lib/python3/dist-packages/apt_pkg.cpython-312-x86_64-linux-gnu.so \
    /usr/lib/python3/dist-packages/apt_pkg.cpython-313-x86_64-linux-gnu.so
  ```

  In case of Windows, it's necessary to install `VS Build tools` <https://bazel.build/configure/windows#clang>.

  Even if the `VS Build tools` can download LLVM toolchain, the instllation is useless as:

  - bazel cannot reach the binaries, probably, untill you specify the exact path to the directory at special env variable
  - the intallation does not provide clangd, which should be present for IDE c++ support

  LLMV can be insalled manually by downloading of a pre-build LLVM archive
    and unpacking it to the `C:\Program Files\LLVM`directory, which should be discovered bazel.

  ```txt
  C:\Program Files\LLVM
    bin/
      clang++.exe
      ...
    include/
    ...
  ```

  **NOTE**: check that bazel finds an actual version of the llvm; it could be so that you can have an old version
    of clang 12.X.X which was destributed using windows installers.

- how to install rust:

  NOTE: Even if Bazel loads the hermetic toolchain, it doesn't expose rustfmt to the sysroot.
    Therefore, it's necessary to add an external formatter binary to enable formatting in the IDE.

  ```sh
  # https://doc.rust-lang.org/book/ch01-01-installation.html
  curl --proto '=https' --tlsv1.2 https://sh.rustup.rs -sSf | sh
  ```

- how to integrate CMake with clangd:

  <https://github.com/SpaceIm/vscode-cpp-cmake-conan-template/blob/conan-v2-ninja-no-presets/.vscode/settings.json>

- how to integrate bazel with clangd:

  <https://github.com/hedronvision/bazel-compile-commands-extractor>
