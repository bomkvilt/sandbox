use crate::physics::Particle;

/// Relative coordinate system for improved numerical precision
pub struct RelativeCoordinates {
    central_body_idx: usize,
}

impl RelativeCoordinates {
    pub fn new(particles: &[Particle]) -> Self {
        // Find the most massive body
        let central_body_idx = particles
            .iter()
            .enumerate()
            .max_by(|(_, a), (_, b)| a.mass.partial_cmp(&b.mass).unwrap())
            .map_or(0, |(idx, _)| idx);

        Self { central_body_idx }
    }

    pub fn to_relative(&self, particles: &mut [Particle]) {
        let central_pos = particles[self.central_body_idx].position;
        let central_vel = particles[self.central_body_idx].velocity;

        for (i, p) in particles.iter_mut().enumerate() {
            if i != self.central_body_idx {
                p.position -= central_pos;
                p.velocity -= central_vel;
            }
        }
    }

    pub fn to_absolute(&self, particles: &mut [Particle]) {
        let central_pos = particles[self.central_body_idx].position;
        let central_vel = particles[self.central_body_idx].velocity;

        for (i, p) in particles.iter_mut().enumerate() {
            if i != self.central_body_idx {
                p.position += central_pos;
                p.velocity += central_vel;
            }
        }
    }
}
