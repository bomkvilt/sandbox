# Massive Gravity V1

The sketch was inspired by the project: <https://github.com/DeadlockCode/quarkstrom/tree/master>

## Setup

### Setup.WSL

- Install an **Ubuntu 24.04 WSL2** disto or later (older versions were unstable in terms of created windows)

- Install Vulkan drivers:

  ```sh
  sudo apt install mesa-vulkan-drivers
  ```

- Add some cratches

  ```sh
  export WAYLAND_DISPLAY=
  ```

- Run the executable:

  ```sh
  WGPU_BACKEND=vulkan bazelisk run :bin
  ```
