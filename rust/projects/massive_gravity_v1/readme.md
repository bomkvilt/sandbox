# Massive Gravity V1

The sketch was inspired by the project: <https://github.com/DeadlockCode/quarkstrom/tree/master>

## Setup

### Setup.WSL

- Install an **Ubuntu 24.04 WSL2** disto or later (older versions were unstable in terms of created windows)

- Install Vulkan drivers:

  ```sh
  sudo apt install mesa-vulkan-drivers
  ```

- Run the executable:

  ```sh
  WAYLAND_DISPLAY="" WGPU_BACKEND=vulkan bazelisk run :bin

  # NOTE: WAYLAND_DISPLAY="" is required to fight the error: WaylandError(Connection(NoCompositor))
  # NOTE: WGPU_BACKEND=vulkan is required to prevent OpenGL backend usage which has issues on WSL
  ```
