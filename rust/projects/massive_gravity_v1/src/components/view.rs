use std::sync::Arc;

use wgpu::util::DeviceExt;

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct View {
    position: [f32; 2],
    scale: f32,
    x: u16,
    y: u16,
}

impl Default for View {
    fn default() -> Self {
        Self {
            position: [0.0, 0.0],
            scale: 1.0,
            x: 0,
            y: 0,
        }
    }
}

unsafe impl bytemuck::Pod for View {}
unsafe impl bytemuck::Zeroable for View {}

// =================================================================================================

const SLOT: u32 = 0;

#[derive(Debug)]
pub struct Controller {
    queue: Arc<wgpu::Queue>,

    view: View,
    buffer: wgpu::Buffer,
    bind_layout: wgpu::BindGroupLayout,
    bind_group: wgpu::BindGroup,
}

impl Controller {
    pub fn new(queue: Arc<wgpu::Queue>, device: &wgpu::Device) -> Self {
        let view = View::default();

        let buffer = device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
            label: Some("View Buffer"),
            contents: bytemuck::cast_slice(&[view]),
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
        });

        let bind_layout = device.create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
            label: Some("View Bind Group Layout"),
            entries: &[wgpu::BindGroupLayoutEntry {
                binding: SLOT,
                visibility: wgpu::ShaderStages::VERTEX,
                ty: wgpu::BindingType::Buffer {
                    ty: wgpu::BufferBindingType::Uniform,
                    has_dynamic_offset: false,
                    min_binding_size: None,
                },
                count: None,
            }],
        });

        let bind_group = device.create_bind_group(&wgpu::BindGroupDescriptor {
            label: Some("View Bind Group"),
            layout: &bind_layout,
            entries: &[wgpu::BindGroupEntry {
                binding: SLOT,
                resource: buffer.as_entire_binding(),
            }],
        });

        Self {
            queue,
            view,
            buffer,
            bind_layout,
            bind_group,
        }
    }

    pub fn layout(&self) -> &wgpu::BindGroupLayout {
        &self.bind_layout
    }

    pub fn bind(&self) -> &wgpu::BindGroup {
        &self.bind_group
    }

    pub fn set_size(&mut self, x: u32, y: u32) {
        self.view.x = u16::try_from(x).unwrap();
        self.view.y = u16::try_from(y).unwrap();
    }

    pub fn sync(&mut self) {
        self.queue
            .write_buffer(&self.buffer, 0, bytemuck::cast_slice(&[self.view]));
    }
}
