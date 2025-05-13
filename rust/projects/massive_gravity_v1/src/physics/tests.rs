#![allow(unused_imports)] // TODO: remove

use crate::physics::cpu::DirectN2Simulator;
use crate::physics::{Particle, Simulator, WorldStateView, G};
use nalgebra::Vector2;
use num_traits::cast::FromPrimitive;

// =================================================================================================

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

fn calculate_total_momentum(particles: &[Particle]) -> Vector2<f64> {
    particles.iter().map(|p| p.mass * p.velocity).sum()
}

fn calculate_total_angular_momentum(particles: &[Particle]) -> f64 {
    let cross = |a: &Vector2<f64>, b: &Vector2<f64>| a.x * b.y - a.y * b.x;
    particles.iter().map(|p| p.mass * cross(&p.position, &p.velocity)).sum()
}

#[derive(Debug)]
struct PeriodTracker {
    phase: f64,

    swept: f64,
    prev_swept: f64,

    time: f64,
    prev_time: f64,

    period: Option<f64>,
}

impl PeriodTracker {
    pub fn new(pos: Vector2<f64>, time: f64) -> Self {
        Self {
            phase: pos.y.atan2(pos.x),
            swept: 0.0,
            prev_swept: 0.0,
            time,
            prev_time: time,
            period: None,
        }
    }

    pub fn update(&mut self, pos: Vector2<f64>, time: f64) -> bool {
        let phase = pos.y.atan2(pos.x);

        let delta_phase = {
            let mut delta = phase - self.phase;
            if delta > std::f64::consts::PI {
                delta -= 2.0 * std::f64::consts::PI;
            } else if delta < -std::f64::consts::PI {
                delta += 2.0 * std::f64::consts::PI;
            }
            delta
        };

        self.prev_swept = self.swept;
        self.swept += delta_phase;
        self.phase = phase;

        self.prev_time = self.time;
        self.time = time;

        if !self.is_completed() {
            return false;
        }

        self.period = Some(self.compute_period());

        std::println!("Orbiter swept: {self:?}");

        true
    }

    pub fn is_completed(&self) -> bool {
        self.swept >= Self::target_swept()
    }

    pub fn get_period(&self) -> Option<f64> {
        self.period
    }

    pub fn get_swept(&self) -> f64 {
        self.swept
    }

    const fn target_swept() -> f64 {
        2.0 * std::f64::consts::PI
    }

    fn compute_period(&self) -> f64 {
        let overshoot = self.swept - Self::target_swept();
        let delta_swept = self.swept - self.prev_swept;
        let fraction = 1.0 - (overshoot / delta_swept);

        let time_step = self.time - self.prev_time;
        self.prev_time + (time_step * fraction)
    }
}

// =================================================================================================

mod model {
    use super::*;

    // Central body parameters
    pub const CENTRAL_MASS: f64 = 100_000.0;
    pub const CENTRAL_RADIUS: f64 = 1.0;
    pub const CENTRAL_ID: usize = 0;
    pub const MU: f64 = G * CENTRAL_MASS;

    // Orbiting body parameters
    pub const ORBITER_MASS: f64 = 1.0;
    pub const ORBITER_RADIUS: f64 = 0.1;
    pub const ORBITER_ID: usize = 1;
    pub const ORBITAL_RADIUS: f64 = 100.0;

    // Test parameters
    pub const STEPS_PER_ORBIT: usize = 500;
    pub const MAX_ORBIT_STEPS: usize = 1000;
    pub const POSITION_TOLERANCE: f64 = 1e-6;

    pub const TOLERANCE_ENERGY_RATE: f64 = 1e-6;
    pub const TOLERANCE_MOMENTUM_RATE: f64 = 1e-10;
    pub const TOLERANCE_ANG_MOMENTUM_RATE: f64 = 1e-10;

    pub const TOLERANCE_PERIOD_RATE: f64 = 1e-3;
    pub const TOLERANCE_RADIUS_RATE: f64 = 1e-3;

    pub struct OrbitalParams {
        pub orbital_velocity: f64,
        pub orbital_period: f64,
        pub time_step: f64,
    }

    pub fn create_test_system() -> (OrbitalParams, Vec<Particle>) {
        let orbital_velocity: f64 = (MU / ORBITAL_RADIUS).sqrt();
        let orbital_period: f64 = 2.0 * std::f64::consts::PI * ORBITAL_RADIUS / orbital_velocity;
        let time_step: f64 = orbital_period / STEPS_PER_ORBIT as f64;

        let mut particles = vec![
            // Central body at origin
            Particle {
                id: CENTRAL_ID,
                position: Vector2::zeros(),
                velocity: Vector2::zeros(),
                acceleration: Vector2::zeros(),
                mass: CENTRAL_MASS,
                radius: CENTRAL_RADIUS,
            },
            // Orbiting body with circular orbit velocity
            Particle {
                id: ORBITER_ID,
                position: Vector2::new(ORBITAL_RADIUS, 0.0),
                velocity: Vector2::new(0.0, orbital_velocity),
                acceleration: Vector2::zeros(),
                mass: ORBITER_MASS,
                radius: ORBITER_RADIUS,
            },
        ];

        (OrbitalParams { orbital_velocity, orbital_period, time_step }, particles)
    }
}

// =================================================================================================

fn run_test_conservation<S: Simulator>(simulator: &mut S) {
    // Initialize the simulator
    let (params, particles) = model::create_test_system();
    simulator.init(WorldStateView { particles, ..Default::default() });

    let init_energy = calculate_total_energy(&simulator.get_state().particles);
    let init_momentum = calculate_total_momentum(&simulator.get_state().particles);
    let init_ang_momentum = calculate_total_angular_momentum(&simulator.get_state().particles);

    // Evolve the system
    for _ in 0..model::STEPS_PER_ORBIT {
        simulator.step(params.time_step);
    }

    // Check conservation laws
    let energy = calculate_total_energy(&simulator.get_state().particles);
    let energy_diff = (energy - init_energy).abs() / init_energy;
    assert!(
        energy_diff < model::TOLERANCE_ENERGY_RATE,
        "Energy conservation violated: {}% change",
        energy_diff * 100.0
    );

    let momentum = calculate_total_momentum(&simulator.get_state().particles);
    let momentum_diff = (momentum - init_momentum).norm() / init_momentum.norm();
    assert!(
        momentum_diff < model::TOLERANCE_MOMENTUM_RATE,
        "Momentum conservation violated: {}% change",
        momentum_diff * 100.0
    );

    let ang_momentum = calculate_total_angular_momentum(&simulator.get_state().particles);
    let ang_momentum_diff = (ang_momentum - init_ang_momentum).abs() / init_ang_momentum.abs();
    assert!(
        ang_momentum_diff < model::TOLERANCE_ANG_MOMENTUM_RATE,
        "Angular momentum conservation violated: {}% change",
        ang_momentum_diff * 100.0
    );
}

fn run_test_period<S: Simulator>(simulator: &mut S) {
    // Initialize the simulator
    let (params, particles) = model::create_test_system();
    simulator.init(WorldStateView { particles, ..Default::default() });

    // Evolve the system and track some system properties
    let init_pos: Vector2<f64> = simulator.get_state().particles[model::ORBITER_ID].position;
    let mut time: f64 = 0.0;

    let mut tracker = PeriodTracker::new(init_pos, time);
    let mut period = None;

    let mut steps = 0;
    while steps < model::MAX_ORBIT_STEPS {
        simulator.step(params.time_step);
        time += params.time_step;

        let pos: Vector2<f64> = simulator.get_state().particles[model::ORBITER_ID].position;

        // Check orbital shape
        let radius_diff = (pos.norm() - model::ORBITAL_RADIUS) / model::ORBITAL_RADIUS;
        assert!(
            radius_diff.abs() < model::TOLERANCE_RADIUS_RATE,
            "Orbiter radius/expected: {radius_diff:.8}",
        );

        // Check if the orbit is complete
        if tracker.update(pos, time) {
            period = tracker.get_period();
            break;
        }

        steps += 1;
    }

    assert!(
        period.is_some(),
        "Orbiter did not complete an orbit within {} steps",
        model::MAX_ORBIT_STEPS
    );

    // Compare with theoretical orbital period
    let period_diff = (period.unwrap() - params.orbital_period) / params.orbital_period;
    assert!(
        period_diff.abs() < model::TOLERANCE_PERIOD_RATE,
        "Orbital period deviates too much from expected value. Expected: {:.0} s, Got: {:.0} s, Diff: {:.2} %",
        params.orbital_period,
        period.unwrap(),
        period_diff * 100.0,
    );
}

// =================================================================================================

#[test]
fn test_direct_n2_conservation() {
    let mut simulator = DirectN2Simulator::new();
    run_test_conservation(&mut simulator);
}

#[test]
fn test_direct_n2_period() {
    let mut simulator = DirectN2Simulator::new();
    run_test_period(&mut simulator);
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
