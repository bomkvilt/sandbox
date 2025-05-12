use crate::physics::integrators::{Integrator, StateRepr, VelocityVerlet};
use crate::physics::{Particle, Simulator, WorldStateView, G};
use nalgebra::Vector2;

#[derive(Debug, Clone, Default)]
struct State {
    particles: Vec<Particle>,
}

impl State {
    fn update_accelerations(&mut self) {
        let particles = &mut self.particles;

        // Reset accelerations
        for p in particles.iter_mut() {
            p.acceleration = Vector2::zeros();
        }

        // Calculate accelerations between all pairs of particles
        for i in 0..particles.len() {
            let (l, r) = particles.split_at_mut(i + 1);

            for j in 0..r.len() {
                let p1 = &mut l[i];
                let p2 = &mut r[j];

                let r = p2.position - p1.position;
                let r_mag = r.norm();

                if r_mag < 1e-10 {
                    continue;
                }

                let force = G * p1.mass * p2.mass * r / (r_mag * r_mag * r_mag);
                p1.acceleration += force / p1.mass;
                p2.acceleration -= force / p2.mass;
            }
        }
    }
}

impl StateRepr for State {
    fn get_particles_mut(&mut self) -> &mut [Particle] {
        &mut self.particles
    }

    fn update_accelerations(&mut self) {
        self.update_accelerations();
    }
}

// =================================================================================================

pub struct DirectN2Simulator {
    state: State,
    integrator: VelocityVerlet,
    timestamp: f64,
}

impl DirectN2Simulator {
    pub fn new() -> Self {
        Self { state: State::default(), integrator: VelocityVerlet, timestamp: 0.0 }
    }
}

impl Simulator for DirectN2Simulator {
    fn init(&mut self, world_state: WorldStateView) {
        self.state.particles = world_state.particles;
        self.timestamp = world_state.timestamp;
        self.state.update_accelerations();
    }

    fn step(&mut self, dt: f64) {
        // Perform integration step
        self.integrator.step(&mut self.state, dt);

        // Update simulation time and accumulated time
        self.timestamp += dt;
    }

    fn get_state(&self) -> WorldStateView {
        WorldStateView { particles: self.state.particles.clone(), timestamp: self.timestamp }
    }
}
