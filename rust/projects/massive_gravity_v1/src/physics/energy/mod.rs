use crate::physics::{Particle, G};

/// Energy conservation utilities
pub struct EnergyConservation {
    initial_energy: f64,
    energy_tolerance: f64,
}

impl EnergyConservation {
    pub fn new(particles: &[Particle], tolerance: f64) -> Self {
        Self {
            initial_energy: Self::calculate_total_energy(particles),
            energy_tolerance: tolerance,
        }
    }

    pub fn check_and_correct(&self, particles: &mut [Particle]) {
        let current_energy = Self::calculate_total_energy(particles);
        let relative_error = (current_energy - self.initial_energy) / self.initial_energy.abs();

        if relative_error.abs() > self.energy_tolerance {
            // Scale velocities to restore energy conservation
            let scale_factor = (self.initial_energy / current_energy).sqrt();
            for p in particles.iter_mut() {
                p.velocity *= scale_factor;
            }
        }
    }

    fn calculate_total_energy(particles: &[Particle]) -> f64 {
        // Calculate kinetic energy: Σ(1/2 * m * v²)
        let kinetic_energy: f64 =
            particles.iter().map(|p| 0.5 * p.mass * p.velocity.norm_squared()).sum();

        // Calculate potential energy: Σ(G * m1 * m2 / r)
        let mut potential_energy = 0.0;
        for i in 0..particles.len() {
            for j in (i + 1)..particles.len() {
                let p1 = &particles[i];
                let p2 = &particles[j];
                let r = (p2.position - p1.position).norm();

                if r < 1e-10 {
                    continue;
                }
                potential_energy -= G * p1.mass * p2.mass / r;
            }
        }

        kinetic_energy + potential_energy
    }
}
