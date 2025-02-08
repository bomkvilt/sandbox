# Massive Gravity V1

The sketch was inspired by the project: <https://github.com/DeadlockCode/quarkstrom/tree/master>

## Setup

### Setup.WSL

- install Vulkan drivers:

  ```sh
  sudo apt install mesa-vulkan-drivers
  ```

- Run the binary:

  ```sh
  WGPU_BACKEND=vulkan bazelisk run :bin
  ```
