use crate::physics::Particle;

/// Adaptive time stepping utilities
pub struct AdaptiveTimeStep {
    base_time_step: f64,
    min_time_step: f64,
    safety_factor: f64,
}

impl AdaptiveTimeStep {
    pub fn new(base_time_step: f64, min_time_step: f64, safety_factor: f64) -> Self {
        Self { base_time_step, min_time_step, safety_factor }
    }

    pub fn calculate_step(&self, particles: &[Particle]) -> f64 {
        // Find maximum acceleration and minimum distance
        let mut max_acceleration: f64 = 0.0;
        let mut min_distance: f64 = f64::MAX;

        for i in 0..particles.len() {
            max_acceleration = max_acceleration.max(particles[i].acceleration.norm());

            for j in (i + 1)..particles.len() {
                let distance = (particles[j].position - particles[i].position).norm();
                min_distance = min_distance.min(distance);
            }
        }

        if max_acceleration > 1e-10 {
            let characteristic_time = (min_distance / max_acceleration).sqrt();
            (self.base_time_step)
                .min(characteristic_time * self.safety_factor)
                .max(self.min_time_step)
        } else {
            self.base_time_step
        }
    }
}
