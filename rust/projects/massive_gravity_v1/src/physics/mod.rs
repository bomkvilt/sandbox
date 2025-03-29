// External dependencies
use nalgebra::Vector3;

// Submodule declarations
pub mod coordinates;
pub mod cpu;
pub mod energy;
pub mod integrators;
pub mod timestep;

// Common physics types and constants
pub const G: f64 = 6.67430e-11; // Gravitational constant (m^3 kg^-1 s^-2)

/// Represents a particle in the gravity simulation
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

/// The world state containing all particles
#[derive(Debug, Clone)]
pub struct WorldState {
    pub particles: Vec<Particle>,
    pub timestamp: f64, // Simulation time in seconds
    pub time_step: f64,
    pub min_time_step: f64,
}

impl WorldState {
    pub fn new() -> Self {
        Self { particles: Vec::new(), timestamp: 0.0, time_step: 1.0, min_time_step: 1e-6 }
    }

    pub fn with_capacity(capacity: usize) -> Self {
        Self {
            particles: Vec::with_capacity(capacity),
            timestamp: 0.0,
            time_step: 1.0,
            min_time_step: 1e-6,
        }
    }

    pub fn add_particle(&mut self, particle: Particle) {
        self.particles.push(particle);
    }

    pub fn as_view(&self) -> WorldStateView {
        WorldStateView { particles: self.particles.clone(), timestamp: self.timestamp }
    }
}

/// A read-only view of the world state for rendering
#[derive(Debug, Clone)]
pub struct WorldStateView {
    pub particles: Vec<Particle>,
    pub timestamp: f64,
}

/// Configuration for simulation stability
#[derive(Debug, Clone)]
pub struct SimulationConfig {
    pub energy_tolerance: f64,
    pub adaptive_time_step: bool,
    pub use_relative_coordinates: bool,
    pub energy_check_interval: usize,
    pub safety_factor: f64,
    pub time_step: f64,
}

impl Default for SimulationConfig {
    fn default() -> Self {
        Self {
            energy_tolerance: 1e-6,
            adaptive_time_step: true,
            use_relative_coordinates: true,
            energy_check_interval: 100,
            safety_factor: 0.1,
            time_step: 1.0,
        }
    }
}

/// Enhanced trait with stability improvements
pub trait GravitySimulator {
    fn initialize(&mut self, world_state: WorldState, config: SimulationConfig);
    fn step(&mut self, dt: f64);
    fn get_world_state(&self) -> WorldStateView;
}

/// Enum for different gravity simulation methods
#[derive(Debug, Clone, Copy)]
pub enum SimulationMethod {
    /// Direct calculation of forces between all particles (O(n²))
    DirectN2,

    /// Barnes-Hut tree-based approximation
    BarnesHut { theta: f64 },
}

/// Main simulation controller that manages time steps and the simulation algorithm
pub struct Simulation {
    simulator: Box<dyn GravitySimulator>,
    config: SimulationConfig,
}

impl Simulation {
    pub fn new(
        method: SimulationMethod,
        initial_state: WorldState,
        time_step: f64,
        min_time_step: f64,
        config: SimulationConfig,
    ) -> Self {
        let mut simulator: Box<dyn GravitySimulator> = match method {
            SimulationMethod::DirectN2 => Box::new(cpu::DirectN2Simulator::new()),
            SimulationMethod::BarnesHut { theta } => Box::new(cpu::BarnesHutSimulator::new(theta)),
        };

        // Initialize the simulator with the initial state and config
        simulator.initialize(initial_state, config.clone());

        Self { simulator, config }
    }

    pub fn step(&mut self) {
        self.simulator.step(self.config.time_step);
    }

    pub fn get_world_state(&self) -> WorldStateView {
        self.simulator.get_world_state()
    }
}

#[cfg(test)]
mod examples;

#[cfg(test)]
mod tests;
