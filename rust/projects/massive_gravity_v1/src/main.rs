// TODO: remove the pragmas
#![allow(dead_code, unused_variables, unused_mut, clippy::unused_self)]

use std::sync::Arc;
use winit::dpi::PhysicalSize;
use winit::event::WindowEvent;
use winit::event_loop::{ActiveEventLoop, ControlFlow, EventLoop};
use winit::window::{Window, WindowId};

mod components;

#[derive(Debug)]
struct State {
    window: Arc<Window>,
    device: wgpu::Device,
    surface: wgpu::Surface<'static>,
    config: wgpu::SurfaceConfiguration,
    queue: Arc<wgpu::Queue>,

    view: components::view::Controller,
    circles: components::circles::Collection,
    circles_pipeline: components::circles::Pipeline,
}

impl State {
    #[allow(clippy::too_many_lines)]
    async fn new(window: Arc<Window>) -> State {
        let mut size = window.inner_size();
        size.width = size.width.max(1);
        size.height = size.height.max(1);

        log::error!("new size: {size:?}");

        // TODO: use `wgpu::util::instance_descriptor_from_env()`
        #[allow(clippy::default_trait_access)]
        let instance = wgpu::Instance::new(wgpu::InstanceDescriptor {
            backends: wgpu::util::backend_bits_from_env().unwrap_or_default(),
            dx12_shader_compiler: Default::default(),
            gles_minor_version: Default::default(),
            flags: Default::default(),
        });

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

        let config = surface
            .get_default_config(&adapter, size.width, size.height)
            .unwrap();
        surface.configure(&device, &config);

        let view = components::view::Controller::new(queue.clone(), &device);

        let circles_pipeline =
            components::circles::Pipeline::new(&device, &surface, &config, &view);

        let mut circles = components::circles::Collection::new(queue.clone(), &device);
        circles.set_instances(&[
            components::circles::Instance {
                position: [0.0, 0.0],
                radius: 0.1,
                color: [250, 0, 0, 0],
            },
            components::circles::Instance {
                position: [0.5, 0.0],
                radius: 0.1,
                color: [200, 0, 0, 0],
            },
            components::circles::Instance {
                position: [0.5, 0.5],
                radius: 0.1,
                color: [150, 0, 0, 0],
            },
            components::circles::Instance {
                position: [0.0, 0.5],
                radius: 0.1,
                color: [100, 0, 0, 0],
            },
        ]);

        Self {
            window,
            device,
            surface,
            config,
            queue,
            view,
            circles,
            circles_pipeline,
        }
    }

    fn resize(&mut self, new_size: PhysicalSize<u32>) {
        // Reconfigure the surface with the new size
        self.config.width = new_size.width.max(1);
        self.config.height = new_size.height.max(1);
        self.surface.configure(&self.device, &self.config);

        // Update viewport size
        self.view.set_size(self.config.width, self.config.height);
        self.view.sync();

        // On macos the window needs to be redrawn manually after resizing
        self.window.request_redraw();
    }

    fn redraw(&mut self) {
        let frame = self
            .surface
            .get_current_texture()
            .expect("Failed to acquire next swap chain texture");

        let view = frame
            .texture
            .create_view(&wgpu::TextureViewDescriptor::default());

        let mut encoder = self
            .device
            .create_command_encoder(&wgpu::CommandEncoderDescriptor { label: None });

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

            rpass.set_pipeline(self.circles_pipeline.pipeline());
            rpass.set_bind_group(0, self.view.bind(), &[]);
            self.circles.render(&mut rpass);
        }

        self.queue.submit(Some(encoder.finish()));
        frame.present();
    }
}

// =================================================================================================

#[derive(Default)]
struct App {
    state: Option<State>,
    window: Option<Arc<Window>>,
}

impl App {
    fn state(&mut self) -> &mut State {
        if self.state.is_none() {
            let window = self.window.as_ref().unwrap().clone();
            self.state = Some(pollster::block_on(State::new(window)));
        }
        return self.state.as_mut().unwrap();
    }
}

impl winit::application::ApplicationHandler for App {
    fn resumed(&mut self, event_loop: &ActiveEventLoop) {
        log::error!("call: resume");
        let window = event_loop
            .create_window(Window::default_attributes())
            .unwrap();
        self.window = Some(window.into());
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
            }
            _ => (),
        }
    }
}

pub fn main() {
    env_logger::init();
    log::error!("init()");

    let event_loop = EventLoop::new().unwrap();
    event_loop.set_control_flow(ControlFlow::Poll);
    event_loop.set_control_flow(ControlFlow::Wait);

    let mut app = App::default();
    let _ = event_loop.run_app(&mut app);
}
