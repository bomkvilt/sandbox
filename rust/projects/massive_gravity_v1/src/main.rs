use std::sync::Arc;
use wgpu::{Device, InstanceDescriptor, InstanceFlags, Queue, Surface, SurfaceConfiguration};
use winit::application::ApplicationHandler;
use winit::dpi::PhysicalSize;
use winit::event::WindowEvent;
use winit::event_loop::{ActiveEventLoop, ControlFlow, EventLoop};
use winit::window::{Window, WindowId};

mod components;

struct State {
    window: Arc<Window>,
    device: Device,
    surface: Surface<'static>,
    config: SurfaceConfiguration,
    queue: Arc<Queue>,

    circles: components::circles::Collection,
    circles_pipeline: components::circles::Pipeline,
}

impl State {
    #[allow(clippy::too_many_lines)]
    async fn new(window: Arc<Window>) -> State {
        let mut size = window.inner_size();
        size.width = size.width.max(1);
        size.height = size.height.max(1);

        // TODO: use `wgpu::util::instance_descriptor_from_env()`
        let instance = wgpu::Instance::new(InstanceDescriptor {
            backends: wgpu::util::backend_bits_from_env().unwrap_or_default(),
            flags: InstanceFlags::from_build_config().with_env(),
            dx12_shader_compiler: wgpu::util::dx12_shader_compiler_from_env().unwrap_or_default(),
            gles_minor_version: wgpu::util::gles_minor_version_from_env().unwrap_or_default(),
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

        let circles_pipeline = components::circles::Pipeline::new(&device, &surface, &adapter);

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
            circles,
            circles_pipeline,
        }
    }

    fn resize(&mut self, new_size: PhysicalSize<u32>) {
        // Reconfigure the surface with the new size
        self.config.width = new_size.width.max(1);
        self.config.height = new_size.height.max(1);
        self.surface.configure(&self.device, &self.config);

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
            self.circles.render(&mut rpass);
        }

        self.queue.submit(Some(encoder.finish()));
        frame.present();
    }
}

#[derive(Default)]
struct App {
    state: Option<State>,
}

impl ApplicationHandler for App {
    fn resumed(&mut self, event_loop: &ActiveEventLoop) {
        log::error!("call: resume");
        let window = event_loop
            .create_window(Window::default_attributes())
            .unwrap();
        self.state = Some(pollster::block_on(State::new(window.into())));
    }

    fn window_event(&mut self, event_loop: &ActiveEventLoop, _id: WindowId, event: WindowEvent) {
        log::error!("call: event: {event:?}");
        match event {
            WindowEvent::CloseRequested => {
                event_loop.exit();
            }
            WindowEvent::Resized(new_size) => {
                self.state
                    .as_mut()
                    .expect("resumed was not called")
                    .resize(new_size);
            }
            WindowEvent::RedrawRequested => {
                self.state
                    .as_mut()
                    .expect("resumed was not called")
                    .redraw();
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
