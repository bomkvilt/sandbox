use crate::physics::G;
use crate::physics::{
    coordinates::RelativeCoordinates,
    energy::EnergyConservation,
    integrators::{Integrator, VelocityVerlet},
    timestep::AdaptiveTimeStep,
    GravitySimulator, SimulationConfig, WorldState, WorldStateView,
};
use nalgebra::Vector3;

pub struct DirectN2Simulator {
    world_state: WorldState,
    integrator: VelocityVerlet,
    energy_conservation: Option<EnergyConservation>,
    adaptive_time_step: Option<AdaptiveTimeStep>,
    relative_coords: Option<RelativeCoordinates>,
    config: SimulationConfig,
    step_count: usize,
    accumulated_time: f64,
}

impl DirectN2Simulator {
    pub fn new() -> Self {
        Self {
            world_state: WorldState::new(),
            integrator: VelocityVerlet,
            energy_conservation: None,
            adaptive_time_step: None,
            relative_coords: None,
            config: SimulationConfig::default(),
            step_count: 0,
            accumulated_time: 0.0,
        }
    }

    fn calculate_accelerations(&mut self) -> Vec<Vector3<f64>> {
        let mut accelerations = vec![Vector3::zeros(); self.world_state.particles.len()];
        let particles = &self.world_state.particles;

        // Calculate accelerations between all pairs of particles
        for i in 0..particles.len() {
            for j in (i + 1)..particles.len() {
                let p1 = &particles[i];
                let p2 = &particles[j];

                let r = p2.position - p1.position;
                let r_mag = r.norm();

                if r_mag < 1e-10 {
                    continue;
                }

                let force = G * p1.mass * p2.mass * r / (r_mag * r_mag * r_mag);
                accelerations[i] += force / p1.mass;
                accelerations[j] -= force / p2.mass;
            }
        }

        accelerations
    }
}

impl GravitySimulator for DirectN2Simulator {
    fn initialize(&mut self, world_state: WorldState, config: SimulationConfig) {
        self.world_state = world_state;
        self.config = config;
        self.accumulated_time = 0.0;

        // Initialize stability improvements
        if self.config.use_relative_coordinates {
            self.relative_coords = Some(RelativeCoordinates::new(&self.world_state.particles));
        }

        self.energy_conservation = Some(EnergyConservation::new(
            &self.world_state.particles,
            self.config.energy_tolerance,
        ));

        self.adaptive_time_step = Some(AdaptiveTimeStep::new(
            self.world_state.time_step,
            self.world_state.min_time_step,
            self.config.safety_factor,
        ));

        // Initial acceleration calculation
        self.calculate_accelerations();
    }

    fn step(&mut self, dt: f64) {
        // Accumulate time
        self.accumulated_time += dt;

        // Perform fixed-size steps to catch up with the accumulated time
        while self.accumulated_time >= self.world_state.time_step {
            // Convert to relative coordinates if enabled
            if let Some(ref coords) = self.relative_coords {
                coords.to_relative(&mut self.world_state.particles);
            }

            // Calculate adaptive time step if enabled
            let step_dt = if let Some(ref adaptive) = self.adaptive_time_step {
                adaptive.calculate_step(&self.world_state.particles).min(self.world_state.time_step)
            } else {
                self.world_state.time_step
            };

            // Calculate accelerations once
            let accelerations = self.calculate_accelerations();

            // Perform integration step
            self.integrator.integrate_step(
                &mut self.world_state.particles,
                step_dt,
                &|particles| {
                    // Apply pre-calculated accelerations
                    for (i, p) in particles.iter_mut().enumerate() {
                        p.acceleration = accelerations[i];
                    }
                },
            );

            // Periodically check and correct energy conservation
            self.step_count += 1;
            if self.step_count % self.config.energy_check_interval == 0 {
                if let Some(ref energy) = self.energy_conservation {
                    energy.check_and_correct(&mut self.world_state.particles);
                }
            }

            // Convert back to absolute coordinates if needed
            if let Some(ref coords) = self.relative_coords {
                coords.to_absolute(&mut self.world_state.particles);
            }

            // Update simulation time and accumulated time
            self.world_state.timestamp += step_dt;
            self.accumulated_time -= step_dt;
        }
    }

    fn get_world_state(&self) -> WorldStateView {
        self.world_state.as_view()
    }
}
