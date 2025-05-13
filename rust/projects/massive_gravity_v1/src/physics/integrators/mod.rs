use crate::physics::Particle;

/// Trait for different integration methods
pub trait Integrator {
    fn step<T: StateRepr>(&self, state: &mut T, dt: f64);
}

pub trait StateRepr {
    fn get_particles_mut(&mut self) -> &mut [Particle];
    fn update_accelerations(&mut self);
}

// =================================================================================================

/// Velocity Verlet integrator for better energy conservation
/// <https://en.wikipedia.org/wiki/Verlet_integration#Velocity_Verlet>
pub struct VelocityVerlet;

impl Integrator for VelocityVerlet {
    fn step<T: StateRepr>(&self, state: &mut T, dt: f64) {
        // First half-step update of velocities and full update of positions
        for p in state.get_particles_mut().iter_mut() {
            // v(t+dt/2) = v(t) + a(t) * dt/2
            p.velocity += p.acceleration * (dt / 2.0);
            // r(t+dt) = r(t) + v(t+dt/2) * dt
            p.position += p.velocity * dt;
        }

        // Recalculate accelerations at new positions
        state.update_accelerations();

        // Second half-step update of velocities
        for p in state.get_particles_mut().iter_mut() {
            // v(t+dt) = v(t+dt/2) + a(t+dt) * dt/2
            p.velocity += p.acceleration * (dt / 2.0);
        }
    }
}
