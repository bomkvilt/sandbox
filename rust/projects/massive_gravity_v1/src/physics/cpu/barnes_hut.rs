use crate::physics::{
    coordinates::RelativeCoordinates,
    energy::EnergyConservation,
    integrators::{Integrator, VelocityVerlet},
    timestep::AdaptiveTimeStep,
    GravitySimulator, Particle, SimulationConfig, WorldState, WorldStateView, G,
};
use nalgebra::Vector3;

const QUADTREE_CHILDREN: usize = 4;

#[derive(Clone)]
struct Node {
    center: Vector3<f64>,
    size: f64,
    total_mass: f64,
    center_of_mass: Vector3<f64>,
    children: Option<[Option<Box<Node>>; QUADTREE_CHILDREN]>,
    particle: Option<Particle>,
}

impl Node {
    fn new_leaf(center: Vector3<f64>, size: f64, particle: Particle) -> Self {
        Self {
            center,
            size,
            total_mass: particle.mass,
            center_of_mass: particle.position,
            children: None,
            particle: Some(particle),
        }
    }

    fn new_internal(center: Vector3<f64>, size: f64) -> Self {
        Self {
            center,
            size,
            total_mass: 0.0,
            center_of_mass: Vector3::zeros(),
            children: Some([None, None, None, None]),
            particle: None,
        }
    }

    fn is_leaf(&self) -> bool {
        self.particle.is_some()
    }

    fn calculate_force(&self, particle: &Particle, theta: f64) -> Vector3<f64> {
        // Skip if this is the same particle
        if let Some(node_particle) = &self.particle {
            if node_particle.id == particle.id {
                return Vector3::zeros();
            }
        }

        let r = self.center_of_mass - particle.position;
        let r_mag = r.norm();

        if r_mag < 1e-10 {
            return Vector3::zeros();
        }

        // Check if we can use approximation
        if self.size / r_mag < theta || self.is_leaf() {
            // Use approximation - consider the self node as a single particle
            let force = G * self.total_mass * particle.mass * r / (r_mag * r_mag * r_mag);
            force / particle.mass
        } else if let Some(children) = &self.children {
            // Recursively calculate forces from children
            children.iter().flatten().map(|child| child.calculate_force(particle, theta)).sum()
        } else {
            std::unreachable!("Invalid node state");
        }
    }
}

fn insert_particle(node: &mut Node, particle: Particle) {
    // Determine which quadrant the particle belongs to
    let mut quadrant = 0;
    if particle.position.x >= node.center.x {
        quadrant |= 1;
    }
    if particle.position.y >= node.center.y {
        quadrant |= 2;
    }

    // Create children array if it doesn't exist
    if node.children.is_none() {
        node.children = Some([None, None, None, None]);
    }

    let children = node.children.as_mut().unwrap();

    // Insert into child node
    if let Some(child) = &mut children[quadrant] {
        if child.is_leaf() {
            // Convert leaf to internal node
            let existing_particle = child.particle.take().unwrap();
            let mut new_internal = Node::new_internal(child.center, child.size);

            // Insert both particles
            insert_particle(&mut new_internal, existing_particle);
            insert_particle(&mut new_internal, particle);
            children[quadrant] = Some(Box::new(new_internal));
        } else {
            // Already an internal node, insert recursively
            insert_particle(child, particle);
        }
    } else {
        // Create new leaf node
        children[quadrant] = Some(Box::new(Node::new_leaf(node.center, node.size / 2.0, particle)));
    }
}

pub struct BarnesHutSimulator {
    world_state: WorldState,
    theta: f64,
    tree: Option<Node>,
    integrator: VelocityVerlet,
    energy_conservation: Option<EnergyConservation>,
    adaptive_time_step: Option<AdaptiveTimeStep>,
    relative_coords: Option<RelativeCoordinates>,
    config: SimulationConfig,
    step_count: usize,
}

impl BarnesHutSimulator {
    pub fn new(theta: f64) -> Self {
        Self {
            world_state: WorldState::new(),
            theta,
            tree: None,
            integrator: VelocityVerlet,
            energy_conservation: None,
            adaptive_time_step: None,
            relative_coords: None,
            config: SimulationConfig::default(),
            step_count: 0,
        }
    }

    fn build_tree(&mut self) {
        let particles = &self.world_state.particles;
        if particles.is_empty() {
            self.tree = None;
            return;
        }

        // Find bounding box
        let mut min_pos = particles[0].position;
        let mut max_pos = particles[0].position;
        for p in particles {
            for i in 0..3 {
                min_pos[i] = min_pos[i].min(p.position[i]);
                max_pos[i] = max_pos[i].max(p.position[i]);
            }
        }

        // Calculate center and size
        let center = (min_pos + max_pos) / 2.0;
        let size = (max_pos - min_pos).norm() * 1.1; // Add 10% margin

        // Create root node
        let mut root = Node::new_internal(center, size);

        // Insert all particles
        for p in particles {
            insert_particle(&mut root, p.clone());
        }

        // Calculate center of mass for all nodes
        self.calculate_center_of_mass(&mut root);

        self.tree = Some(root);
    }

    fn calculate_center_of_mass(&self, node: &mut Node) {
        if let Some(children) = &node.children {
            let mut total_mass = 0.0;
            let mut weighted_pos = Vector3::zeros();

            for child in children.iter().flatten() {
                let child_mass = child.total_mass;
                let child_com = child.center_of_mass;

                total_mass += child_mass;
                weighted_pos += child_com * child_mass;
            }

            if total_mass > 0.0 {
                node.total_mass = total_mass;
                node.center_of_mass = weighted_pos / total_mass;
            }
        }
    }

    fn calculate_acceleration_for_particle(
        &self,
        particle_idx: usize,
        tree: &Node,
    ) -> Vector3<f64> {
        let particle = &self.world_state.particles[particle_idx];
        tree.calculate_force(particle, self.theta)
    }

    fn calculate_accelerations(&mut self) -> Vec<Vector3<f64>> {
        // Reset accelerations
        let mut accelerations = vec![Vector3::zeros(); self.world_state.particles.len()];

        // Build the tree
        self.build_tree();

        // Calculate accelerations using the tree
        if let Some(tree) = &self.tree {
            for (i, acc) in accelerations.iter_mut().enumerate() {
                *acc = self.calculate_acceleration_for_particle(i, tree);
            }
        }

        accelerations
    }
}

impl GravitySimulator for BarnesHutSimulator {
    fn initialize(&mut self, world_state: WorldState, config: SimulationConfig) {
        self.world_state = world_state;
        self.config = config;

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
        // Convert to relative coordinates if enabled
        if let Some(ref coords) = self.relative_coords {
            coords.to_relative(&mut self.world_state.particles);
        }

        // Calculate adaptive time step if enabled
        let effective_dt = if let Some(ref adaptive) = self.adaptive_time_step {
            adaptive.calculate_step(&self.world_state.particles)
        } else {
            dt
        };

        // Calculate accelerations once
        let accelerations = self.calculate_accelerations();

        // Perform integration step
        self.integrator.integrate_step(
            &mut self.world_state.particles,
            effective_dt,
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

        // Update simulation time
        self.world_state.timestamp += effective_dt;
    }

    fn get_world_state(&self) -> WorldStateView {
        self.world_state.as_view()
    }
}
