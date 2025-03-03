# Architecture

## Main purpose

This is an educational project, which main goal is to bring me a practice of GPU programming in Rust.

As long as gravity simulation is a very wide topic and can incapsulate huge variety of space scales from
  starting from single objects to galaxy-scale simulation of millions to billions of particles.
And the problem classes requires different computation architectures with specific incompatible optimisations,
  as long as different solving methods:

- direct ODE system solving
- tree-based approximation (e.g. Barnes-Hut)
- grid-based approximation (e.g. P3M; e.g. [click](<https://en.num-meth.ru/index.php/journal/article/view/1250>))

In this project, I will be focused on the first two methods as they are more about **interactive simulation**,
  rather than a scientific simulation.

More specifically, the project should implement the following:

- gravity simulation engine
- particles rendering facilities
- GUI overlay for simulation parameters and interaction
- player-controlled with particle interaction capabilities

<!-- =========================================================================================== -->

## Data Conveyor

The data conveyor should support a scaled real-time simulation.

### Time model

In this project we focus on a real-time simulation.
This means that we should be able to generate a reasonable frame rate, meanwhile the simulation's time
  should be scaled to a wall-time ($t\_scale = \frac{\Delta t_{simulation}}{\Delta t_{wall}}$).

For simplicity, let's assume that our pipeline consists of two asynchronous stages:
  (1) simulation ($t_{simulation}$) and (2) rendering ($t_{rendering}$) —
  and have a rps limiting with a minimum frame time ($t_{min}$).

Let's also assume that the simulation stage is able to produce new world stages very quickly.
In this case, our frame time will be equal to the $t_{rendering}$.

If the rendering is quick enough ($t_{rendering} < t_{min}$), the rendering stage will just sleep
  the remaining time.

If the rendering is slow ($t_{rendering} > t_{min}$), observable frame will just slow down,
  but the simlation will not increase its lag behind the real time.

But if the simlation will lose its ability to catch up with the real time, the simulation will
  stop being real-time.
This scenario is possible when the simlation works longer than $\Delta t_{wall}$ to simulate
  the wall time $\Delta t_{wall}$.

But even in this case, the simlation will still remain interactable, and will look like a real-time
  simulation in case of some code modifications:

- effective `now()` vs. wall-time `now()`

And if the effective now() is used, there should (or should not) be used a catch up mechanism
  that should increase the simlation time when the simlation speeed turns back to real-time step timings.

List of time variables:

- $t_{wall}$ — wall-time clocks (units: wall seconds)
- $t_{effective}$ — effective simulation wall-time time (units: wall seconds)
- $t_{simulation}$ — simulation time (units: simulation seconds)

### Simulation model

In general we have two major types of simulation models:

- Simulation with fixed-time steps (simulation-first, fixed-size steps)
- Simulation with variadic-time steps (real-time, single-staged, $dt = max(t_{curr} - t_{prev}, dt_{min})$)

And also we have a combination of both: semi-fixed time steps (multiple fixed-size steps per
  frame with a delay accumulator for error correction purposes).

In terms of a rendering, the simulated world always delays from the real world, as it should be.
To prevent this effect, the simulated world positions could be interpolated or extrapolated to
  compensate the delay.

- Interpolation: we **simulate** the world **in advance** and interpolate object positions between
  last (two) simlated states to make a smooth transition to the $t_{effective}$.
- Extrapolation: we **simulate** the world **with a delay** and extrapolate object positions
  to catch up with the $t_{effective}$.

The smoothing techniques are similar and both should fit well for simulation purposes,
  but it is not obvious whether the smoothing is really needed.
The smoothing can **be added to the physics stage** if needed.

```py
@final
class Simulation:
  def __init__(self, time_step: datetime.timedelta, min_time_step: datetime.timedelta):
    self.__world_state = WorldState()  # TODO: expose
    self.__accumulated_time = datetime.timedelta(0)
    self.__min_time_step = min_time_step
    self.__time_step = time_step

  def step(self, delta_time: datetime.timedelta): # delta_time is a simulation time
    # avoid small time steps
    effective_time_step: Final = max(self.__time_step, self.__accumulated_time)

    # TODO: add error correction
    self.__accumulated_time += delta_time

    # perform a bunch of small fixed-size steps to catch up with the time
    while self.__accumulated_time >= self.__time_step:
      self.__world_state.update(self.__time_step)
      self.__accumulated_time -= self.__time_step

  def get_world_state(self) -> WorldStateView:
    """
    Returns a constant view of the world state.
    """
    return self.__world_state.as_view()


simulation: Final = Simulation()

while True:
  ...
  now = datetime.datetime.now()
  simulation_delta_time = (now - last_frame_time).total_seconds() * t_scale
  last_frame_time = now

  simulation.step(simulation_delta_time)
```

### Rendering pipeline

- Window redrawing can be requested by calling the `request_redraw` method

- The `request_redraw` method is not provided with deduplication mechanisms
  - resulting frame rate is not predictable
  - event queue can be flooded with redraw requests and can continiously grow without not limits

As a result it sounds safer to separate rendering and simulation stages.
And once the stages are separated, they should be galvanically isolated as well,
  which can be implemented in terms of queues or `std::atomic<std::unique_ptr<...>>` built
  on top of object pools (to prevent memory allocations).

The isolation will mean additional latency (0.0..1.0 frame time), which cannot be completely
  worked around even by using event system and frame rendering worker pool, which start new
  frame preparatoin as soon as new free worker is ready, as if the rendering time is increasing
  the simlation time, no free workers will be instantly available and queue waiting time will be
  in the range of 0.0..1.0 frame time.
But the worker pool approach should have smaller latency than a completely asyncronous rendering stage.

Thus, we have a three-stage pipeline:

```text
Simulation stage:
- simulates the world
- prepares world snapshots for rendering

:interface: `std::atomic<std::unique_ptr<...>>` -- last world snapshot
:interface: conditional variable

Rendering stage:
- consists of a worker pool where each worker:
  - waits for a new world snapshot
  - prepares a new frame
  - requests redrawing

:interface: `std::atomic<std::unique_ptr<...>>` -- last frame

Redrawing stage:
- sets the frame to a current frame buffer
```

### List of libraries for consideration

- typed-arena: For arena allocation
- bumpalo: For arena allocation
- nalgebra: For linear algebra and physics calculations
- rayon: For parallel computation
- crossbeam: For lock-free data structures
- dashmap: For concurrent hash maps
- slice-pool: For efficient reuse of vector-like data structures
