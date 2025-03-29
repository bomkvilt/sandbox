use crate::physics::Particle;

/// Trait for different integration methods
pub trait Integrator {
    fn integrate_step(
        &self,
        particles: &mut [Particle],
        dt: f64,
        calculate_accelerations: &dyn Fn(&mut [Particle]),
    );
}

/// Velocity Verlet integrator for better energy conservation
pub struct VelocityVerlet;

impl Integrator for VelocityVerlet {
    fn integrate_step(
        &self,
        particles: &mut [Particle],
        dt: f64,
        calculate_accelerations: &dyn Fn(&mut [Particle]),
    ) {
        // First half-step update of velocities and full update of positions
        for p in particles.iter_mut() {
            // v(t+dt/2) = v(t) + a(t) * dt/2
            p.velocity += p.acceleration * (dt / 2.0);
            // r(t+dt) = r(t) + v(t+dt/2) * dt
            p.position += p.velocity * dt;
        }

        // Recalculate accelerations at new positions
        calculate_accelerations(particles);

        // Second half-step update of velocities
        for p in particles.iter_mut() {
            // v(t+dt) = v(t+dt/2) + a(t+dt) * dt/2
            p.velocity += p.acceleration * (dt / 2.0);
        }
    }
}
