use crate::physics::G;
use crate::physics::{Particle, Vector3, WorldState};
use rand::Rng;

fn derive_mean_velocity(r: &Vector3<f64>, mu: f64) -> f64 {
    (mu / r.norm()).sqrt()
}

/// Creates a solar system simulation with the Sun and planets
pub fn create_solar_system() -> WorldState {
    const AU: f64 = 149.6e9;
    const MS: f64 = 1.989e30;
    const MU: f64 = G * MS;

    let mut world = WorldState::new();

    // Sun
    world.add_particle(Particle::new(
        Vector3::new(0.0, 0.0, 0.0),
        Vector3::new(0.0, 0.0, 0.0),
        MS,        // Sun's mass in kg
        696_340e3, // Sun's radius in m
        0,
    ));

    // Earth
    {
        let position = Vector3::new(AU, 0.0, 0.0);
        let velocity = Vector3::new(0.0, derive_mean_velocity(&position, MU), 0.0);
        world.add_particle(Particle::new(
            position, velocity, 5.972e24, // Earth's mass in kg
            6371e3,   // Earth's radius in m
            1,
        ));
    }

    // Mars
    // {
    //     let position = Vector3::new(1.52 * AU, 0.0, 0.0);
    //     let velocity = Vector3::new(0.0, derive_mean_velocity(&position, MU), 0.0);
    //     world.add_particle(Particle::new(
    //         position, velocity, 6.39e23,  // Mars's mass in kg
    //         3389.5e3, // Mars's radius in m
    //         2,
    //     ));
    // }

    world
}

/// Creates a random particle system for testing
pub fn create_random_system(num_particles: usize) -> WorldState {
    let mut world = WorldState::new();
    let mut rng = rand::thread_rng();

    // Add a central massive body
    world.add_particle(Particle::new(
        Vector3::new(0.0, 0.0, 0.0),
        Vector3::new(0.0, 0.0, 0.0),
        1e30, // Central mass
        1e9,  // Central radius
        0,
    ));

    // Add random particles
    for i in 1..num_particles {
        // Random position in a sphere
        let radius = rng.gen_range(1e9..1e11);
        let theta = rng.gen_range(0.0..2.0 * std::f64::consts::PI);
        let phi = rng.gen_range(0.0..std::f64::consts::PI);

        let x = radius * phi.sin() * theta.cos();
        let y = radius * phi.sin() * theta.sin();
        let z = radius * phi.cos();

        // Calculate orbital velocity for roughly circular orbits
        let orbital_speed = (G * 1e30 / radius).sqrt();
        let vx = -orbital_speed * y / radius;
        let vy = orbital_speed * x / radius;
        let vz = 0.0;

        world.add_particle(Particle::new(
            Vector3::new(x, y, z),
            Vector3::new(vx, vy, vz),
            1e24, // Particle mass
            1e6,  // Particle radius
            i,
        ));
    }

    world
}
