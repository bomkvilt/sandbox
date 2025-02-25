// NOTE: a triangle with an inscribed unit circle
var<private> VERTICES: array<vec2<f32>, 3> = array<vec2<f32>, 3>(
    vec2<f32>(-1.7321, -1.0),
    vec2<f32>( 1.7321, -1.0), // sqrt(3) ≈ 1.7321
    vec2<f32>( 0.0   ,  2.0),
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
};

// Screen Size:
//          y
// *--------|--------* (1, h/w = 1/aspect)
// |        |        |
// |        0----------> x
// |                 |
// *-----------------*
//
// Pixel Size: (clip space)
// x: 2 / w
// y: 2 / h = 2 / (w / aspect) = (2 / w) * aspect
//
struct View {
    position: vec2<f32>, // center of the view (world units)

    scale: f32,     // world units per screen unit
    inv_scale: f32, // 1 / scale

    aspect: f32,     // = w / h; pre-computed aspect ratio
    inv_aspect: f32, // = h / w; pre-computed inverse aspect ratio

    // circle size that inscribes one pixel: 1 / w (view units)
    // - w pixels occupies 2 view units, 1 pixel = 2 / w view units
    // - the minimum curcle inscribes the pixel
    min_radius: f32,
};

@group(0) @binding(0)
var<uniform> view: View;

@vertex
fn vs_main(vertex: VertexInput, instance: InstanceInput) -> VertexOutput {
    var out: VertexOutput;
    out.color = instance.color;

    var repaired_radius = instance.radius;
    var repaired_position = instance.position;

    // handle the case when a tringle is maler than a pixel
    let min_world_radus = view.min_radius * view.scale;
    if instance.radius < min_world_radus {
        // snap to the grid
        let view_position = (instance.position - view.position) * view.inv_scale;
        repaired_position = floor(instance.position / min_world_radus) * min_world_radus; // NOTE: y sill not aligned

        // dim the color as S[x] / S[min]
        let scale = instance.radius / min_world_radus;
        out.color[3] *= scale * scale;
        repaired_radius = min_world_radus;
    }

    let local_space = VERTICES[vertex.index];
    out.local_space = local_space;

    let world_space = local_space * repaired_radius + repaired_position;
    let view_space = (world_space - view.position) * view.inv_scale;
    out.clip_space = vec4<f32>(view_space.x, view_space.y * view.aspect, 0.0, 1.0);
    return out;
}

@fragment
fn fs_main(vertex: VertexOutput) -> @location(0) vec4<f32> {
    if length(vertex.local_space) > 1.0 {
        discard;
    }
    return vertex.color;
}
