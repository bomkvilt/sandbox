use std::sync::Arc;

use winit::dpi::PhysicalPosition;
use winit::event::{MouseScrollDelta, TouchPhase, WindowEvent};

use crate::components;

pub trait World {
    fn handle_event(&self, event: &WindowEvent) {}
}

// =================================================================================================

const SPEED: f32 = 0.1;

pub struct Symulation {
    camera: Arc<components::camera::Camera>,
}

impl Symulation {
    pub fn new(camera: Arc<components::camera::Camera>) -> Self {
        Self { camera }
    }

    #[allow(clippy::cast_possible_truncation)]
    fn handle_camera_move(&self, delta: PhysicalPosition<f64>) {
        {
            let mut view = self.camera.view.lock().unwrap();
            view.position[0] -= delta.x as f32 * SPEED;
            view.position[1] += delta.y as f32 * SPEED;
        }
        self.camera.sync();
    }

    fn handle_camera_zoom(&self, zoom: f32) {
        {
            let mut view = self.camera.view.lock().unwrap();
            view.scale *= 1.0 - zoom * 0.1;
        }
        self.camera.sync();
    }
}

impl World for Symulation {
    fn handle_event(&self, event: &WindowEvent) {
        match event {
            WindowEvent::MouseWheel { delta, phase, .. } if *phase == TouchPhase::Moved => {
                match delta {
                    MouseScrollDelta::LineDelta(_, zoom) => self.handle_camera_zoom(*zoom),
                    MouseScrollDelta::PixelDelta(delta) => self.handle_camera_move(*delta),
                    #[allow(unreachable_patterns)]
                    _ => {}
                }
            }
            _ => {}
        }
    }
}
