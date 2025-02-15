use std::sync::Arc;

use winit::dpi::{PhysicalPosition, PhysicalSize};
use winit::event::{MouseScrollDelta, TouchPhase, WindowEvent};

use crate::components;

// Coordinate systms:
//                   y
//        0--------- W / 2 ---- W -> x
// Y      |          |
// |      |          |
// |      H/2        0-----------> x
// |      |          View Space coordinates (units)
// |      |
// |      H
// |      |
// |      y Screen Space coordinates (pixels)
// |
// | World Space coordinates (units)
// 0-----------------------> X
//

pub trait World {
    fn handle_event(&mut self, event: &WindowEvent) {}
}

// =================================================================================================

const ZOOM_SPEED: f32 = 0.1;

pub struct Simulation {
    camera: Arc<components::camera::Camera>,
    drag_controller: DragController,
    window_controller: WindowController,
}

impl Simulation {
    pub fn new(camera: Arc<components::camera::Camera>) -> Self {
        Self {
            camera,
            drag_controller: DragController::new(),
            window_controller: WindowController::new(),
        }
    }

    #[allow(clippy::cast_possible_truncation)]
    fn handle_camera_move(&self, view_delta: PhysicalPosition<f64>) {
        {
            let mut view = self.camera.view.lock().unwrap();
            view.position[0] -= view_delta.x as f32 * view.scale();
            view.position[1] -= view_delta.y as f32 * view.scale() * view.inv_aspect();
        }
        self.camera.sync();
    }

    fn handle_camera_zoom(&self, zoom: f32) {
        {
            let mut view = self.camera.view.lock().unwrap();
            let scale = view.scale();
            view.set_scale(scale - scale * zoom.clamp(-1.0, 1.0) * ZOOM_SPEED);
        }
        self.camera.sync();
    }
}

impl World for Simulation {
    #[allow(clippy::collapsible_match)]
    #[allow(clippy::match_wildcard_for_single_variants)]
    #[allow(clippy::single_match)]
    fn handle_event(&mut self, event: &WindowEvent) {
        match event {
            WindowEvent::MouseWheel { delta, phase, .. } if *phase == TouchPhase::Moved => {
                match delta {
                    MouseScrollDelta::LineDelta(_, zoom) => self.handle_camera_zoom(*zoom),
                    _ => {}
                }
            }
            WindowEvent::MouseInput { state, button, .. } => {
                if *button == winit::event::MouseButton::Left {
                    let pressed = *state == winit::event::ElementState::Pressed;
                    self.drag_controller.snap(pressed);
                }
            }
            WindowEvent::CursorMoved { position, .. } => {
                // handle drags
                self.drag_controller.track(*position);
                if self.drag_controller.is_active() {
                    let screen_delta = self.drag_controller.delta();
                    let view_delta = self.window_controller.delta_screen_to_view(screen_delta);
                    if let Some(view_delta) = view_delta {
                        self.handle_camera_move(view_delta);
                    }
                }
            }
            WindowEvent::Resized(size) => {
                self.window_controller.resize(*size);
            }
            _ => {}
        }
    }
}

// =================================================================================================

struct WindowController {
    window_size: PhysicalSize<u32>,
}

impl WindowController {
    pub fn new() -> Self {
        Self { window_size: PhysicalSize::new(0, 0) }
    }

    pub fn resize(&mut self, size: PhysicalSize<u32>) {
        self.window_size = size;
    }

    pub fn size(&self) -> PhysicalSize<u32> {
        self.window_size
    }

    /// Convert screen-space coordinates to view-space coordinates without scaling (pixels -> [0, 1]).
    pub fn delta_screen_to_view(
        &self,
        delta: PhysicalPosition<f64>,
    ) -> Option<PhysicalPosition<f64>> {
        if self.window_size.width == 0 || self.window_size.height == 0 {
            return None;
        }
        Some(PhysicalPosition::new(
            2.0 * delta.x / f64::from(self.window_size.width),
            -2.0 * delta.y / f64::from(self.window_size.height),
        ))
    }
}

/// The Controller tracks drag mouse events in screen-space coordinates.
struct DragController {
    prev_position: PhysicalPosition<f64>,
    curr_position: PhysicalPosition<f64>,
    active: bool,
}

impl DragController {
    pub fn new() -> Self {
        Self {
            prev_position: PhysicalPosition::new(0.0, 0.0),
            curr_position: PhysicalPosition::new(0.0, 0.0),
            active: false,
        }
    }

    pub fn snap(&mut self, active: bool) {
        self.active = active;
    }

    pub fn is_active(&self) -> bool {
        self.active
    }

    pub fn track(&mut self, position: PhysicalPosition<f64>) {
        self.prev_position = self.curr_position;
        self.curr_position = position;
    }

    /// The methods is valid only if the controller is active.
    pub fn delta(&self) -> PhysicalPosition<f64> {
        PhysicalPosition::new(
            self.curr_position.x - self.prev_position.x,
            self.curr_position.y - self.prev_position.y,
        )
    }
}
