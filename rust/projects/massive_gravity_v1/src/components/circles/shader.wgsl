// NOTE: a triangle with an inscribed circle
var<private> VERTICES: array<vec2<f32>, 3> = array<vec2<f32>, 3>(
    vec2<f32>(-1.7321,-1.0),
    vec2<f32>( 1.7321,-1.0), // sqrt(3) ≈ 1.7321
    vec2<f32>( 0.0   , 2.0),
);

struct VertexInput {
    @builtin(vertex_index) index: u32
};

struct InstanceInput {
    @location(0) position: vec2<f32>,
    @location(1) radius: f32,
    @location(2) color: vec4<f32>,
};

struct VertexOutput {
    @builtin(position) clip_space: vec4<f32>,
    @location(0) local_space: vec2<f32>,
    @location(1) color: vec4<f32>,
    // @location(2) pixel_size: f32,
};

struct View {
    position: vec2<f32>,
    scale: f32,
    x: u32,
    y: u32,
};

@group(0) @binding(0)
var<uniform> view: View;

@vertex
fn vs_main(vertex: VertexInput, instance: InstanceInput) -> VertexOutput {
    var out: VertexOutput;
    out.color = instance.color;

    let aspect = f32(view.y) / f32(view.x);

    let local_space = VERTICES[vertex.index];
    out.local_space = local_space;

    let world_space = local_space * instance.radius + instance.position;
    let view_space = (world_space - view.position) / view.scale;
    out.clip_space = vec4<f32>(view_space.x * aspect, view_space.y, 0.0, 1.0);
    return out;
}

@fragment
fn fs_main(vertex: VertexOutput) -> @location(0) vec4<f32> {
    if length(vertex.local_space) > 1 {
        discard;
    }
    return vertex.color;
}
