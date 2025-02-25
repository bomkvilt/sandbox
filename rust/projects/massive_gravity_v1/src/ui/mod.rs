use imgui::{Condition, Context, FontSource};
use imgui_wgpu::{Renderer, RendererConfig};
use imgui_winit_support::{HiDpiMode, WinitPlatform};

/// The handler incapsulates gui layer management
pub struct GuiHandler {
    context: Context,
    platform: WinitPlatform,
    renderer: Renderer,

    // TODO: remove (move the test flags on a user-defined contrller level)
    value: f32,
    opened: bool,
}

impl GuiHandler {
    #[allow(clippy::cast_possible_truncation)]
    pub fn new(
        window: &winit::window::Window,
        texture_format: wgpu::TextureFormat,
        device: &wgpu::Device,
        queue: &wgpu::Queue,
    ) -> Self {
        let hidpi_factor = window.scale_factor();

        let mut context = Context::create();
        context.set_ini_filename(None);

        // setup font
        context.io_mut().font_global_scale = (1.0 / hidpi_factor) as f32;

        let font_size = (13.0 * hidpi_factor) as f32;
        context.fonts().add_font(&[FontSource::DefaultFontData {
            config: Some(imgui::FontConfig {
                oversample_h: 1,
                pixel_snap_h: true,
                size_pixels: font_size,
                ..Default::default()
            }),
        }]);

        let mut platform = WinitPlatform::new(&mut context);
        platform.attach_window(context.io_mut(), window, HiDpiMode::Default);

        // setup renderer
        let renderer_config = RendererConfig { texture_format, ..Default::default() };
        let renderer = Renderer::new(&mut context, device, queue, renderer_config);

        Self { context, platform, renderer, value: 5.0, opened: false }
    }
}

// =================================================================================================
// Event handling
// =================================================================================================

impl GuiHandler {
    /// Handles window events and returns true if the event was captured by the UI
    pub fn handle_window_event(
        &mut self,
        window: &winit::window::Window,
        event: &winit::event::WindowEvent,
    ) -> bool {
        self.platform.handle_event::<()>(
            self.context.io_mut(),
            window,
            &winit::event::Event::WindowEvent { window_id: window.id(), event: event.clone() },
        );

        #[allow(clippy::needless_return, clippy::match_same_arms)]
        match event {
            winit::event::WindowEvent::MouseInput { .. } => {
                return self.context.io().want_capture_mouse;
            }
            winit::event::WindowEvent::MouseWheel { .. } => {
                return self.context.io().want_capture_mouse;
            }
            winit::event::WindowEvent::KeyboardInput { .. } => {
                return self.context.io().want_capture_keyboard;
            }
            _ => {
                return false;
            }
        };
    }

    pub fn handle_user_event(&mut self, window: &winit::window::Window, event: ()) {
        self.platform.handle_event::<()>(
            self.context.io_mut(),
            window,
            &winit::event::Event::UserEvent(event),
        );
    }

    pub fn handle_device_event(
        &mut self,
        window: &winit::window::Window,
        device_id: winit::event::DeviceId,
        event: winit::event::DeviceEvent,
    ) {
        self.platform.handle_event::<()>(
            self.context.io_mut(),
            window,
            &winit::event::Event::DeviceEvent { device_id, event },
        );
    }

    pub fn handle_about_to_wait(&mut self, window: &winit::window::Window) {
        self.platform.handle_event::<()>(
            self.context.io_mut(),
            window,
            &winit::event::Event::AboutToWait,
        );
    }
}

// =================================================================================================
// Rendering
// =================================================================================================

impl GuiHandler {
    pub fn prepare(
        &mut self,
        device: &wgpu::Device,
        queue: &wgpu::Queue,
        window: &winit::window::Window,
        encoder: &mut wgpu::CommandEncoder,
    ) {
        self.context.io_mut().update_delta_time(std::time::Duration::from_millis(100));

        self.platform
            .prepare_frame(self.context.io_mut(), window)
            .expect("Failed to prepare frame");

        let ui = self.context.frame();

        // TODO: move a to user-defined UI controller
        {
            let ui_window = ui.window("Hello world");
            ui_window.size([300.0, 100.0], Condition::FirstUseEver).build(|| {
                ui.text("Hello world!");
                ui.text("This...is...imgui-rs on WGPU!");
                ui.separator();
                let mouse_pos = ui.io().mouse_pos;
                ui.text(format!("Mouse Position: ({:.1},{:.1})", mouse_pos[0], mouse_pos[1]));
            });

            let window = ui.window("Hello too");
            window
                .size([400.0, 200.0], Condition::FirstUseEver)
                .position([400.0, 200.0], Condition::FirstUseEver)
                .build(|| {
                    ui.text("Frametime: 0");
                });

            ui.show_demo_window(&mut self.opened);
        }

        self.platform.prepare_render(ui, window);
    }

    pub fn render<'r>(
        &'r mut self,
        device: &wgpu::Device,
        queue: &wgpu::Queue,
        rpass: &mut wgpu::RenderPass<'r>,
    ) {
        self.renderer
            .render(self.context.render(), queue, device, rpass)
            .expect("Rendering failed");
    }
}
