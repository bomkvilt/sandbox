// TODO: remove the pragmas
#![allow(dead_code, unused_variables, unused_mut, clippy::unused_self)]

use std::borrow::Cow;
use std::sync::Arc;

#[repr(C)]
#[derive(Copy, Clone, Debug)]
pub struct Instance {
    pub position: [f32; 2],
    pub radius: f32,
    pub color: [u8; 4],
}

impl Instance {
    const ATTRIBS: [wgpu::VertexAttribute; 3] =
        wgpu::vertex_attr_array![0 => Float32x2, 1 => Float32, 2 => Unorm8x4];

    fn device_layout<'a>() -> wgpu::VertexBufferLayout<'a> {
        wgpu::VertexBufferLayout {
            array_stride: std::mem::size_of::<Instance>() as wgpu::BufferAddress,
            step_mode: wgpu::VertexStepMode::Instance,
            attributes: &Self::ATTRIBS,
        }
    }
}

unsafe impl bytemuck::Pod for Instance {}
unsafe impl bytemuck::Zeroable for Instance {}

// =================================================================================================

#[derive(Debug)]
pub struct Collection {
    queue: Arc<wgpu::Queue>,
    buffer: wgpu::Buffer,
    count: u32,
}

impl Collection {
    /// `instances` are the first extra buffer
    const SLOT: u32 = 0;

    pub fn new(queue: Arc<wgpu::Queue>, device: &wgpu::Device) -> Self {
        Self {
            queue,
            buffer: device.create_buffer(&wgpu::BufferDescriptor {
                label: Some("circles.buffer"),
                usage: wgpu::BufferUsages::STORAGE
                    | wgpu::BufferUsages::VERTEX
                    | wgpu::BufferUsages::COPY_DST,
                size: 1 << 28, // TODO: use more explict magic constant
                mapped_at_creation: false,
            }),
            count: 0u32,
        }
    }

    pub fn render(&self, rpass: &mut wgpu::RenderPass<'_>) {
        rpass.set_vertex_buffer(Self::SLOT, self.buffer.slice(..));
        rpass.draw(0..3, 0..self.count); // render triangles
    }

    pub fn set_instances(&mut self, instances: &[Instance]) {
        self.count = u32::try_from(instances.len()).expect("too many instances");
        self.queue
            .write_buffer(&self.buffer, 0, bytemuck::cast_slice(instances));
    }
}

// =================================================================================================

#[derive(Debug)]
pub struct Pipeline {
    pipeline: wgpu::RenderPipeline,

    /// The `camera` is required here as the `pipiline` uses the `camera`'s buffers and the `camera`
    /// must overlive the `pipiline`. Thus, the field should be located below the `pipline`
    /// <https://doc.rust-lang.org/reference/destructors.html#r-destructors.operation>
    camera: Arc<super::camera::Camera>,
}

impl Pipeline {
    pub fn new(
        device: &wgpu::Device,
        surface: &wgpu::Surface,
        config: &wgpu::SurfaceConfiguration,
        camera: Arc<super::camera::Camera>,
    ) -> Self {
        // Load the shader from disk
        let shader_file = runfiles::rlocation!(
            runfiles::Runfiles::create().unwrap(),
            "sandbox/rust/projects/massive_gravity_v1/src/components/circles/shader.wgsl"
        );
        let shader = device.create_shader_module(wgpu::ShaderModuleDescriptor {
            label: None,
            source: wgpu::ShaderSource::Wgsl(Cow::Owned(
                std::fs::read_to_string(shader_file.unwrap()).unwrap(),
            )),
        });

        let layout = device.create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
            label: None,
            bind_group_layouts: &[camera.get_layout()],
            push_constant_ranges: &[],
        });

        #[allow(clippy::default_trait_access)]
        let pipeline = device.create_render_pipeline(&wgpu::RenderPipelineDescriptor {
            label: Some("circles.pipeline"),
            layout: Some(&layout),
            vertex: wgpu::VertexState {
                module: &shader,
                entry_point: Some("vs_main"),
                compilation_options: Default::default(),
                buffers: &[Instance::device_layout()],
            },
            fragment: Some(wgpu::FragmentState {
                module: &shader,
                entry_point: Some("fs_main"),
                compilation_options: Default::default(),
                targets: &[Some(wgpu::ColorTargetState {
                    format: config.format,
                    blend: Some(wgpu::BlendState::REPLACE),
                    write_mask: wgpu::ColorWrites::ALL,
                })],
            }),
            primitive: wgpu::PrimitiveState::default(),
            depth_stencil: None,
            multisample: wgpu::MultisampleState::default(),
            multiview: None,
            cache: None,
        });

        Self { pipeline, camera }
    }

    pub fn snap_to_pass(&self, rpass: &mut wgpu::RenderPass<'_>) {
        rpass.set_pipeline(&self.pipeline);
    }
}
