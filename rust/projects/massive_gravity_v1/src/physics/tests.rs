#![allow(unused_imports)] // TODO: remove

use crate::physics::cpu::{BarnesHutSimulator, DirectN2Simulator};
use crate::physics::{GravitySimulator, Particle, SimulationConfig, WorldState, G};
use nalgebra::Vector3;
use num_traits::cast::FromPrimitive;

mod constants {
    use super::*;

    // The system contains a central body and an orbiting body.

    // Central body parameters
    pub const CENTRAL_MASS: f64 = 1000.0;
    pub const CENTRAL_RADIUS: f64 = 1.0;
    pub const CENTRAL_ID: usize = 0;
    pub const MU: f64 = G * CENTRAL_MASS;

    // Orbiting body parameters
    pub const ORBITER_MASS: f64 = 1.0;
    pub const ORBITER_RADIUS: f64 = 0.1;
    pub const ORBITER_ID: usize = 1;
    pub const ORBITAL_RADIUS: f64 = 10.0;

    // Test parameters
    pub const STEPS_PER_ORBIT: usize = 100;
    pub const MAX_ORBIT_STEPS: usize = 1000;
    pub const POSITION_TOLERANCE: f64 = 1e-6;
    pub const ENERGY_TOLERANCE: f64 = 1e-6;
    pub const MOMENTUM_TOLERANCE: f64 = 1e-10;
    pub const ANGULAR_MOMENTUM_TOLERANCE: f64 = 1e-10;
}

use constants::*;

fn calculate_total_energy(particles: &[Particle]) -> f64 {
    let mut total_energy = 0.0;

    // Kinetic energy
    for p in particles {
        total_energy += 0.5 * p.mass * p.velocity.norm_squared();
    }

    // Potential energy
    for i in 0..particles.len() {
        for j in (i + 1)..particles.len() {
            let r = particles[j].position - particles[i].position;
            total_energy -= G * particles[i].mass * particles[j].mass / r.norm();
        }
    }

    total_energy
}

fn calculate_total_momentum(particles: &[Particle]) -> Vector3<f64> {
    particles.iter().map(|p| p.mass * p.velocity).sum()
}

fn calculate_total_angular_momentum(particles: &[Particle]) -> Vector3<f64> {
    particles.iter().map(|p| p.mass * p.position.cross(&p.velocity)).sum()
}

fn create_test_system(orbital_velocity: f64) -> Vec<Particle> {
    let mut particles = vec![
        // Central body at origin
        Particle {
            id: CENTRAL_ID,
            position: Vector3::new(0.0, 0.0, 0.0),
            velocity: Vector3::zeros(),
            acceleration: Vector3::zeros(),
            mass: CENTRAL_MASS,
            radius: CENTRAL_RADIUS,
        },
        // Orbiting body with circular orbit velocity
        Particle {
            id: ORBITER_ID,
            position: Vector3::new(ORBITAL_RADIUS, 0.0, 0.0),
            velocity: Vector3::new(0.0, orbital_velocity, 0.0),
            acceleration: Vector3::zeros(),
            mass: ORBITER_MASS,
            radius: ORBITER_RADIUS,
        },
    ];

    particles
}

fn run_physics_tests<S: GravitySimulator>(simulator: &mut S, config: SimulationConfig) {
    // Calculated constants
    let orbital_velocity: f64 = (MU / ORBITAL_RADIUS).sqrt();
    let orbital_period: f64 = 2.0 * std::f64::consts::PI * ORBITAL_RADIUS / orbital_velocity;
    let time_step: f64 = orbital_period / STEPS_PER_ORBIT as f64;

    let mut world_state = WorldState::new();
    world_state.particles = create_test_system(orbital_velocity);
    simulator.initialize(world_state, config);

    // Test energy conservation
    let initial_energy = calculate_total_energy(&simulator.get_world_state().particles);
    for _ in 0..STEPS_PER_ORBIT {
        simulator.step(time_step);
    }

    let final_energy = calculate_total_energy(&simulator.get_world_state().particles);
    let energy_diff = (final_energy - initial_energy).abs() / initial_energy;

    assert!(
        energy_diff < ENERGY_TOLERANCE,
        "Energy conservation violated: {}% change",
        energy_diff * 100.0
    );

    // Test momentum conservation
    let initial_momentum = calculate_total_momentum(&simulator.get_world_state().particles);
    for _ in 0..STEPS_PER_ORBIT {
        simulator.step(time_step);
    }

    let final_momentum = calculate_total_momentum(&simulator.get_world_state().particles);
    let momentum_diff = (final_momentum - initial_momentum).norm() / initial_momentum.norm();
    assert!(momentum_diff < MOMENTUM_TOLERANCE, "Momentum conservation violated");

    // Test angular momentum conservation
    let initial_angular_momentum =
        calculate_total_angular_momentum(&simulator.get_world_state().particles);
    for _ in 0..STEPS_PER_ORBIT {
        simulator.step(time_step);
    }

    let final_angular_momentum =
        calculate_total_angular_momentum(&simulator.get_world_state().particles);
    let angular_momentum_diff = (final_angular_momentum - initial_angular_momentum).norm()
        / initial_angular_momentum.norm();
    assert!(
        angular_momentum_diff < ANGULAR_MOMENTUM_TOLERANCE,
        "Angular momentum conservation violated"
    );

    // Test orbital period
    let initial_pos = simulator.get_world_state().particles[ORBITER_ID].position;
    let mut steps = 0;
    let mut completed_orbit = false;

    while steps < MAX_ORBIT_STEPS {
        simulator.step(time_step);
        let current_pos = simulator.get_world_state().particles[ORBITER_ID].position;
        let distance = (current_pos - initial_pos).norm();

        println!("distance: {distance}");

        if distance < POSITION_TOLERANCE {
            completed_orbit = true;
            break;
        }
        steps += 1;
    }

    assert!(completed_orbit, "Orbiter did not complete an orbit within {MAX_ORBIT_STEPS} steps");
    assert!(
        (steps as f64 - STEPS_PER_ORBIT as f64).abs() < STEPS_PER_ORBIT as f64 * 0.1,
        "Orbital period deviates too much from expected value"
    );
}

#[test]
fn test_direct_n2() {
    let mut simulator = DirectN2Simulator::new();
    let config = SimulationConfig {
        energy_tolerance: ENERGY_TOLERANCE,
        energy_check_interval: 1,
        ..Default::default()
    };
    run_physics_tests(&mut simulator, config);
}

// #[test]
// fn test_barnes_hut() {
//     let mut simulator = BarnesHutSimulator::new(0.5);
//     let config = SimulationConfig {
//         energy_tolerance: ENERGY_TOLERANCE,
//         energy_check_interval: 1,
//         ..Default::default()
//     };
//     run_physics_tests(&mut simulator, config);
// }

// #[test]
// fn test_barnes_hut_theta_values() {
//     let theta_values = [0.1, 0.5, 1.0];
//     for &theta in &theta_values {
//         let mut simulator = BarnesHutSimulator::new(theta);
//         let config = SimulationConfig {
//             energy_tolerance: ENERGY_TOLERANCE,
//             energy_check_interval: 1,
//             ..Default::default()
//         };
//         run_physics_tests(&mut simulator, config);
//     }
// }

// #[test]
// fn test_simulator_equivalence() {
//     let mut direct_n2 = DirectN2Simulator::new();
//     let mut barnes_hut = BarnesHutSimulator::new(0.5);
//     let config = SimulationConfig {
//         energy_tolerance: ENERGY_TOLERANCE,
//         energy_check_interval: 1,
//         ..Default::default()
//     };

//     // Initialize both simulators with the same initial conditions
//     let mut world_state = WorldState::new();
//     world_state.particles = create_test_system();
//     direct_n2.initialize(world_state.clone(), config);
//     barnes_hut.initialize(world_state, config);

//     // Run both simulators for one orbit
//     for _ in 0..STEPS_PER_ORBIT {
//         direct_n2.step(TIME_STEP);
//         barnes_hut.step(TIME_STEP);
//     }

//     // Compare final states
//     let direct_state = direct_n2.get_world_state();
//     let barnes_state = barnes_hut.get_world_state();

//     for (p1, p2) in direct_state.particles.iter().zip(barnes_state.particles.iter()) {
//         let pos_diff = (p1.position - p2.position).norm();
//         let vel_diff = (p1.velocity - p2.velocity).norm();
//         let acc_diff = (p1.acceleration - p2.acceleration).norm();

//         assert!(pos_diff < POSITION_TOLERANCE, "Position difference too large: {}", pos_diff);
//         assert!(vel_diff < POSITION_TOLERANCE, "Velocity difference too large: {}", vel_diff);
//         assert!(acc_diff < POSITION_TOLERANCE, "Acceleration difference too large: {}", acc_diff);
//     }
// }
