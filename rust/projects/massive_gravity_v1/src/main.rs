// TODO: remove the pragmas
#![allow(dead_code, unused_variables, unused_mut, clippy::unused_self)]

use std::ops::DerefMut;
use std::sync::Arc;
use winit::dpi::PhysicalSize;
use winit::event::WindowEvent;
use winit::event_loop::{ActiveEventLoop, ControlFlow, EventLoop};
use winit::window::{Window, WindowId};

mod components;
mod core;

struct State {
    window: Arc<Window>,
    device: wgpu::Device,
    surface: wgpu::Surface<'static>,
    config: wgpu::SurfaceConfiguration,
    queue: Arc<wgpu::Queue>,

    camera: Arc<components::camera::Camera>,
    circles: components::circles::Collection,
    circles_pipeline: components::circles::Pipeline,
    world: Box<dyn crate::core::World>,
}

impl State {
    #[allow(clippy::too_many_lines)]
    async fn new(window: Arc<Window>) -> State {
        let mut size = window.inner_size();
        size.width = size.width.max(1);
        size.height = size.height.max(1);

        log::error!("new size: {size:?}");

        let instance = wgpu::Instance::new(&wgpu::InstanceDescriptor::from_env_or_default());

        let surface = instance.create_surface(window.clone()).unwrap();

        let adapter = instance
            .request_adapter(&wgpu::RequestAdapterOptions {
                power_preference: wgpu::PowerPreference::default(),
                force_fallback_adapter: false,
                // Request an adapter which can render to our surface
                compatible_surface: Some(&surface),
            })
            .await
            .expect("Failed to find an appropriate adapter");

        // Create the logical device and command queue
        let (device, queue) = adapter
            .request_device(
                &wgpu::DeviceDescriptor {
                    label: None,
                    required_features: wgpu::Features::empty(),
                    required_limits: {
                        wgpu::Limits::downlevel_webgl2_defaults().using_resolution(adapter.limits())
                    },
                    memory_hints: wgpu::MemoryHints::MemoryUsage,
                },
                None,
            )
            .await
            .expect("Failed to create device");

        let queue = Arc::new(queue);

        let config = surface.get_default_config(&adapter, size.width, size.height).unwrap();
        surface.configure(&device, &config);

        let camera = Arc::new(components::camera::Camera::new(queue.clone(), &device));

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
        ]);

        let world = Box::new(crate::core::Symulation::new(camera.clone()));

        Self { window, device, surface, config, queue, camera, circles, circles_pipeline, world }
    }

    fn resize(&mut self, new_size: PhysicalSize<u32>) {
        // Reconfigure the surface with the new size
        self.config.width = new_size.width.max(1);
        self.config.height = new_size.height.max(1);
        self.surface.configure(&self.device, &self.config);

        // Update viewport size
        {
            let mut guard = self.camera.view.lock().unwrap();
            guard.deref_mut().x = self.config.width;
            guard.deref_mut().y = self.config.height;
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

        let view = frame.texture.create_view(&wgpu::TextureViewDescriptor::default());
        {
            let mut rpass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
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
            });

            self.circles_pipeline.snap_to_pass(&mut rpass);
            self.camera.snap_to_pass(&mut rpass);
            self.circles.render(&mut rpass);
        }

        self.queue.submit(Some(encoder.finish()));
        frame.present();
    }

    fn handle_event(&mut self, event_loop: &ActiveEventLoop, event: &WindowEvent) {
        self.world.handle_event(event);
    }
}

// =================================================================================================

#[derive(Default)]
struct Application {
    state: Option<State>,
    window: Option<Arc<Window>>,
}

impl Application {
    fn state(&mut self) -> &mut State {
        // NOTE: It could be wrong, but if we create the state instantly after the window,
        // the frame can have incorrect surface rendering: =~ negative paddings
        if self.state.is_none() {
            let window = self.window.as_ref().unwrap().clone();
            self.state = Some(pollster::block_on(State::new(window)));
        }
        self.state.as_mut().unwrap()
    }
}

impl winit::application::ApplicationHandler for Application {
    fn resumed(&mut self, event_loop: &ActiveEventLoop) {
        log::error!("call: resume");
        let window = event_loop.create_window(Window::default_attributes()).unwrap();
        self.window = Some(window.into());

        // {
        //     let window = self.window.clone();
        //     std::thread::spawn(move || loop {
        //         if let Some(ref window) = window {
        //             std::thread::sleep(std::time::Duration::from_millis(100));
        //             window.request_redraw();
        //         }
        //     });
        // }
    }

    fn window_event(&mut self, event_loop: &ActiveEventLoop, _id: WindowId, event: WindowEvent) {
        log::error!("call: event: {event:?}");
        match event {
            WindowEvent::CloseRequested => {
                event_loop.exit();
            }
            WindowEvent::Resized(new_size) => {
                self.state().resize(new_size);
            }
            WindowEvent::RedrawRequested => {
                self.state().redraw();

                // if let Some(ref window) = self.window {
                //     std::thread::sleep(std::time::Duration::from_millis(100));
                //     window.request_redraw();
                // }
            }
            WindowEvent::MouseInput { .. }
            | WindowEvent::MouseWheel { .. }
            | WindowEvent::KeyboardInput { .. } => {
                self.state().handle_event(event_loop, &event);
            }
            _ => {}
        }
    }
}

pub fn main() {
    env_logger::init();
    log::error!("init()");

    let event_loop = EventLoop::new().unwrap();
    event_loop.set_control_flow(ControlFlow::Poll);
    event_loop.set_control_flow(ControlFlow::Wait);

    let mut app = Application::default();
    let _ = event_loop.run_app(&mut app);
}
