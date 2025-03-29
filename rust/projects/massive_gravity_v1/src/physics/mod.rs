use nalgebra::Vector3;

pub mod cpu;
pub mod integrators;
pub mod timestep;

// Common physics types and constants
pub const G: f64 = 6.67430e-11; // Gravitational constant (m^3 kg^-1 s^-2)

#[derive(Debug, Clone)]
pub struct Particle {
    pub position: Vector3<f64>,
    pub velocity: Vector3<f64>,
    pub acceleration: Vector3<f64>,
    pub mass: f64,
    pub radius: f64, // For collision detection
    pub id: usize,   // Unique identifier
}

impl Particle {
    pub fn new(
        position: Vector3<f64>,
        velocity: Vector3<f64>,
        mass: f64,
        radius: f64,
        id: usize,
    ) -> Self {
        Self { position, velocity, acceleration: Vector3::zeros(), mass, radius, id }
    }
}

// =================================================================================================

#[derive(Debug, Clone)]
pub struct WorldState {
    pub particles: Vec<Particle>,
    pub timestamp: f64, // Simulation time in seconds
}

impl WorldState {
    pub fn new() -> Self {
        Self { particles: Vec::new(), timestamp: 0.0 }
    }

    pub fn add_particle(&mut self, particle: Particle) {
        self.particles.push(particle);
    }

    pub fn as_view(&self) -> WorldStateView {
        WorldStateView { particles: self.particles.clone(), timestamp: self.timestamp }
    }
}

// =================================================================================================

/// A view of the world state for rendering
#[derive(Debug, Clone)]
pub struct WorldStateView {
    pub particles: Vec<Particle>,
    pub timestamp: f64,
}

// =================================================================================================

// TODO: review this
#[derive(Debug, Clone)]
pub struct SimulationConfig {
    pub energy_tolerance: f64,
    pub adaptive_time_step: bool,
    pub energy_check_interval: usize,
    pub safety_factor: f64,
}

impl Default for SimulationConfig {
    fn default() -> Self {
        Self {
            energy_tolerance: 1e-6,
            adaptive_time_step: false,
            energy_check_interval: 100,
            safety_factor: 0.1,
        }
    }
}

// =================================================================================================

trait Simulator {
    fn init(&mut self, world_state: WorldState, config: SimulationConfig);

    /// Perform simulation steps.
    /// The certain time step can be different from the dt provided.
    fn step(&mut self, dt: f64);

    /// Get a view of the world state
    fn get_world_state(&self) -> WorldStateView;
}

#[derive(Debug, Clone, Copy)]
pub enum SimulationMethod {
    /// Direct calculation of forces between all particles (O(n²))
    DirectN2,

    /// Barnes-Hut tree-based approximation
    BarnesHut { theta: f64 },
}

pub struct Simulation {
    simulator: Box<dyn Simulator>,
}

impl Simulation {
    pub fn new(
        method: SimulationMethod,
        world_state: WorldState,
        config: SimulationConfig,
    ) -> Self {
        let mut simulator: Box<dyn Simulator> = match method {
            SimulationMethod::DirectN2 => Box::new(cpu::DirectN2Simulator::new()),
            SimulationMethod::BarnesHut { theta } => Box::new(cpu::BarnesHutSimulator::new(theta)),
        };

        // Initialize the simulator with the initial state and config
        simulator.init(world_state, config);

        Self { simulator }
    }

    pub fn step(&mut self, time_step: f64) {
        self.simulator.step(time_step);
    }

    pub fn get_world_state(&self) -> WorldStateView {
        self.simulator.get_world_state()
    }
}

// =================================================================================================

#[cfg(test)]
mod tests;
