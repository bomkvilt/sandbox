use std::sync::{Arc, Mutex};

use derive_builder::Builder;
use wgpu::util::DeviceExt;

/// NOTE: Pay attention to the structure size (controlled by the _padding)
///
/// documentation: <https://www.w3.org/TR/WGSL/#alignment-and-size>
/// visualisation: <https://webgpufundamentals.org/webgpu/lessons/resources/wgsl-offset-computer.html>
#[repr(C)]
#[derive(Debug, Copy, Clone, Builder, bytemuck_derive::Pod, bytemuck_derive::Zeroable)]
pub struct View {
    #[builder(default)]
    pub position: [f32; 2],

    #[builder(default = 1.0)]
    pub scale: f32,

    #[builder(default)]
    w: u32,

    #[builder(default)]
    h: u32,

    /// Aspect ratio (width / height)
    #[builder(default)]
    aspect: f32,

    /// Inverse aspect ratio (height / width)
    #[builder(default)]
    inv_aspect: f32,

    #[builder(default)]
    _padding: [u8; 4],
}

impl View {
    #[allow(clippy::cast_precision_loss)]
    pub fn set_size(&mut self, w: u32, h: u32) {
        self.w = w.max(1);
        self.h = h.max(1);
        self.aspect = self.w as f32 / self.h as f32;
        self.inv_aspect = self.h as f32 / self.w as f32;
    }

    pub fn w(&self) -> u32 {
        self.w
    }

    pub fn h(&self) -> u32 {
        self.h
    }

    pub fn aspect(&self) -> f32 {
        self.aspect
    }

    pub fn inv_aspect(&self) -> f32 {
        self.inv_aspect
    }
}

// =================================================================================================

const INDEX: u32 = 0;

#[derive(Debug)]
pub struct Camera {
    queue: Arc<wgpu::Queue>,

    /// Viewport specification.
    /// Changes will not be applied unless you call the `sync` method
    pub view: Mutex<View>,

    buffer: wgpu::Buffer,
    layout_handle: wgpu::BindGroupLayout,
    group_handle: wgpu::BindGroup,
}

impl Camera {
    pub fn new(queue: Arc<wgpu::Queue>, device: &wgpu::Device) -> Self {
        let view = ViewBuilder::default().build().unwrap();

        let buffer = device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
            label: Some("View Buffer"),
            contents: bytemuck::cast_slice(&[view]),
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
        });

        let layout_handle = device.create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
            label: Some("View Bind Group Layout"),
            entries: &[wgpu::BindGroupLayoutEntry {
                binding: INDEX,
                visibility: wgpu::ShaderStages::VERTEX,
                ty: wgpu::BindingType::Buffer {
                    ty: wgpu::BufferBindingType::Uniform,
                    has_dynamic_offset: false,
                    min_binding_size: None,
                },
                count: None,
            }],
        });

        let group_handle = device.create_bind_group(&wgpu::BindGroupDescriptor {
            label: Some("View Bind Group"),
            layout: &layout_handle,
            entries: &[wgpu::BindGroupEntry {
                binding: INDEX,
                resource: buffer.as_entire_binding(),
            }],
        });

        Self { queue, view: view.into(), buffer, layout_handle, group_handle }
    }

    pub fn sync(&self) {
        let view = *self.view.lock().unwrap();
        self.queue.write_buffer(&self.buffer, 0, bytemuck::cast_slice(&[view]));
    }

    pub fn get_layout(&self) -> &wgpu::BindGroupLayout {
        &self.layout_handle
    }

    pub fn snap_to_pass(&self, rpass: &mut wgpu::RenderPass<'_>) {
        rpass.set_bind_group(INDEX, &self.group_handle, &[]);
    }
}
