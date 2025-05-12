use nalgebra::Vector2;

pub mod cpu;
pub mod integrators;

pub const G: f64 = 6.67430e-11; // Gravitational constant (m^3 kg^-1 s^-2)

// =================================================================================================

#[derive(Debug, Clone)]
pub struct Particle {
    pub position: Vector2<f64>,
    pub velocity: Vector2<f64>,
    pub acceleration: Vector2<f64>,
    pub mass: f64,
    pub radius: f64, // For collision detection
    pub id: usize,   // Unique identifier
}

impl Particle {
    pub fn new(
        position: Vector2<f64>,
        velocity: Vector2<f64>,
        mass: f64,
        radius: f64,
        id: usize,
    ) -> Self {
        Self { position, velocity, acceleration: Vector2::zeros(), mass, radius, id }
    }
}

// =================================================================================================

#[derive(Debug, Clone, Default)]
pub struct WorldStateView {
    pub particles: Vec<Particle>,
    pub timestamp: f64,
}

// =================================================================================================

trait Simulator {
    fn init(&mut self, world_state: WorldStateView);

    /// Perform simulation steps.
    /// The resulting time can differ from the "current time + dt".
    fn step(&mut self, dt: f64);

    /// Get a view of the world state
    fn get_state(&self) -> WorldStateView;
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
    accumulated_step: f64,
}

impl Simulation {
    pub fn new(method: SimulationMethod, world_state: WorldStateView) -> Self {
        let mut simulator: Box<dyn Simulator> = match method {
            SimulationMethod::DirectN2 => Box::new(cpu::DirectN2Simulator::new()),
            // SimulationMethod::BarnesHut { theta } => Box::new(cpu::BarnesHutSimulator::new(theta)),
            SimulationMethod::BarnesHut { .. } => panic!("Unsupported simulation method"),
        };
        simulator.init(world_state);

        Self { simulator, accumulated_step: 0.0 }
    }

    pub fn step(&mut self, dt: f64) {
        self.accumulated_step += dt;

        // TODO: min_step
        while self.accumulated_step >= dt {
            self.simulator.step(dt);
            self.accumulated_step -= dt;
        }
    }

    pub fn get_world_state(&self) -> WorldStateView {
        self.simulator.get_state()
    }
}

// =================================================================================================

#[cfg(test)]
mod tests;
