// TODO: remove the pragmas
#![allow(dead_code, unused_variables, unused_mut, clippy::unused_self)]

use std::sync::Arc;
use winit::dpi::PhysicalSize;
use winit::event::WindowEvent;
use winit::event_loop::{ActiveEventLoop, ControlFlow, EventLoop};
use winit::window::{Window, WindowId};
mod components;
mod core;
mod physics;
mod ui;

struct AppState {
    window: Arc<Window>,
    device: wgpu::Device,
    surface: wgpu::Surface<'static>,
    config: wgpu::SurfaceConfiguration,
    queue: Arc<wgpu::Queue>,

    camera: Arc<components::camera::Camera>,
    circles: components::circles::Collection,
    circles_pipeline: components::circles::Pipeline,
    world: Box<dyn crate::core::World>,

    gui: ui::GuiHandler,
}

impl AppState {
    #[allow(clippy::too_many_lines)]
    fn new(event_loop: &ActiveEventLoop) -> AppState {
        // create a new window
        let window = {
            let attributes = Window::default_attributes().with_title("Massive Gravity");
            Arc::new(event_loop.create_window(attributes).unwrap())
        };

        // deduct a window size
        let mut size = window.inner_size();
        size.width = size.width.max(1);
        size.height = size.height.max(1);
        log::error!("new size: {size:?}");

        // create a new wgpu instance
        let instance = wgpu::Instance::new(&wgpu::InstanceDescriptor::from_env_or_default());
        let surface = instance.create_surface(window.clone()).unwrap();

        let adapter = {
            pollster::block_on(instance.request_adapter(&wgpu::RequestAdapterOptions {
                power_preference: wgpu::PowerPreference::default(),
                force_fallback_adapter: false,
                // Request an adapter which can render to our surface
                compatible_surface: Some(&surface),
            }))
            .expect("Failed to find an appropriate adapter")
        };

        // Create a logical device and a command queue
        let (device, queue) = {
            pollster::block_on(adapter.request_device(
                &wgpu::DeviceDescriptor {
                    label: None,
                    required_features: wgpu::Features::empty(),
                    required_limits: {
                        wgpu::Limits::downlevel_webgl2_defaults().using_resolution(adapter.limits())
                    },
                    memory_hints: wgpu::MemoryHints::MemoryUsage,
                },
                None,
            ))
            .expect("Failed to create device")
        };
        let queue = Arc::new(queue);

        // setup swap chain
        let config = surface.get_default_config(&adapter, size.width, size.height).unwrap();
        surface.configure(&device, &config);

        // create a virtual camera
        let camera = Arc::new(components::camera::Camera::new(queue.clone(), &device));

        // setup primitives rendering pipeline
        let circles_pipeline =
            components::circles::Pipeline::new(&device, &surface, &config, camera.clone());

        let mut circles = components::circles::Collection::new(queue.clone(), &device);
        circles.set_instances(&[
            components::circles::Instance {
                position: [0.0, 0.0],
                radius: 0.1,
                color: [250, 0, 0, 255],
            },
            components::circles::Instance {
                position: [0.5, 0.0],
                radius: 0.1,
                color: [200, 0, 0, 255],
            },
            components::circles::Instance {
                position: [0.5, 0.5],
                radius: 0.1,
                color: [150, 0, 0, 255],
            },
            components::circles::Instance {
                position: [0.0, 0.5],
                radius: 0.1,
                color: [100, 0, 0, 255],
            },
        ]); // TODO: remove

        let world = Box::new(crate::core::Simulation::new(camera.clone()));

        let gui = ui::GuiHandler::new(&window, config.format, &device, &queue);

        Self {
            window,
            device,
            surface,
            config,
            queue,
            camera,
            circles,
            circles_pipeline,
            world,
            gui,
        }
    }

    fn resize(&mut self, new_size: PhysicalSize<u32>) {
        // Reconfigure the surface with the new size
        self.config.width = new_size.width.max(1);
        self.config.height = new_size.height.max(1);
        self.surface.configure(&self.device, &self.config);

        // Update viewport size
        {
            let mut view = self.camera.view.lock().unwrap();
            view.set_size(self.config.width, self.config.height);
        }
        self.camera.sync();

        // On macos the window needs to be redrawn manually after resizing
        self.window.request_redraw();
    }

    fn acquire(&mut self) -> wgpu::SurfaceTexture {
        match self.surface.get_current_texture() {
            Ok(frame) => frame,
            Err(wgpu::SurfaceError::Timeout) => {
                self.surface.get_current_texture().expect("Failed to acquire next surface texture!")
            }
            Err(..) => {
                // Reconfiguring should help in case of:
                // + wgpu::SurfaceError::Outdated
                // + wgpu::SurfaceError::Lost
                self.surface.configure(&self.device, &self.config);
                self.surface.get_current_texture().expect("Failed to acquire next surface texture!")
            }
        }
    }

    fn redraw(&mut self) {
        let frame = self.acquire();

        let mut encoder =
            self.device.create_command_encoder(&wgpu::CommandEncoderDescriptor { label: None });

        self.gui.prepare(&self.device, &self.queue, &self.window, &mut encoder);

        let view = frame.texture.create_view(&wgpu::TextureViewDescriptor::default());
        {
            let mut rpass = encoder
                .begin_render_pass(&wgpu::RenderPassDescriptor {
                    label: None,
                    color_attachments: &[Some(wgpu::RenderPassColorAttachment {
                        view: &view,
                        resolve_target: None,
                        ops: wgpu::Operations {
                            load: wgpu::LoadOp::Clear(wgpu::Color::GREEN),
                            store: wgpu::StoreOp::Store,
                        },
                    })],
                    depth_stencil_attachment: None,
                    timestamp_writes: None,
                    occlusion_query_set: None,
                })
                .forget_lifetime();

            self.circles_pipeline.snap_to_pass(&mut rpass);
            self.camera.snap_to_pass(&mut rpass);
            self.circles.render(&mut rpass);

            self.gui.render(&self.device, &self.queue, &mut rpass);
        }

        self.queue.submit(Some(encoder.finish()));
        frame.present();
    }

    fn handle_window_event(&mut self, event_loop: &ActiveEventLoop, event: &WindowEvent) {
        // stage 1: handle system events
        match event {
            WindowEvent::CloseRequested => {
                event_loop.exit();
            }
            WindowEvent::Resized(new_size) => {
                self.resize(*new_size);
            }
            WindowEvent::RedrawRequested => {
                self.redraw();

                // TODO: use more correct rps limiting
                {
                    std::thread::sleep(std::time::Duration::from_millis(100));
                    self.window.request_redraw();
                }
            }
            _ => {}
        }

        // stage 2: handle gui events
        if self.gui.handle_window_event(&self.window, event) {
            return;
        }

        // stage 3: handle world events
        self.world.handle_event(event);
    }

    fn handle_user_event(&mut self, event: ()) {
        self.gui.handle_user_event(&self.window, event);
    }

    fn handle_device_event(
        &mut self,
        device_id: winit::event::DeviceId,
        event: winit::event::DeviceEvent,
    ) {
        self.gui.handle_device_event(&self.window, device_id, event);
    }

    fn handle_about_to_wait(&mut self) {
        self.gui.handle_about_to_wait(&self.window);
    }
}

// =================================================================================================

#[derive(Default)]
struct App {
    state: Option<AppState>,
}

impl App {
    fn state(&mut self) -> &mut AppState {
        self.state.as_mut().unwrap()
    }
}

impl winit::application::ApplicationHandler for App {
    fn resumed(&mut self, event_loop: &ActiveEventLoop) {
        self.state = Some(AppState::new(event_loop));
    }

    fn window_event(&mut self, event_loop: &ActiveEventLoop, _id: WindowId, event: WindowEvent) {
        // log::error!("call: window_event: event: {event:?}");
        self.state().handle_window_event(event_loop, &event);
    }

    fn user_event(&mut self, event_loop: &ActiveEventLoop, event: ()) {
        self.state().handle_user_event(event);
    }

    fn device_event(
        &mut self,
        _event_loop: &ActiveEventLoop,
        device_id: winit::event::DeviceId,
        event: winit::event::DeviceEvent,
    ) {
        self.state().handle_device_event(device_id, event);
    }

    fn about_to_wait(&mut self, _event_loop: &ActiveEventLoop) {
        self.state().handle_about_to_wait();
    }
}

pub fn main() {
    env_logger::builder().filter_level(log::LevelFilter::Info).init();

    let event_loop = EventLoop::new().unwrap();
    event_loop.set_control_flow(ControlFlow::Poll);
    event_loop.run_app(&mut App::default()).unwrap();
}
