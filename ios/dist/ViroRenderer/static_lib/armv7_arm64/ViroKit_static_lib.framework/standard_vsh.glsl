#version 300 es

layout (std140) uniform lighting_vertex {
    int lv_num_lights;
    lowp float lv_padding0, lv_padding1, lv_padding2;
    
    mat4 shadow_view_matrices[8];
    mat4 shadow_projection_matrices[8];
};

struct VROShaderGeometry {
    vec3 position;
    vec3 normal;
    vec2 texcoord;
    vec4 tangent;
    vec4 bone_weights;
    ivec4 bone_indices;
} _geometry;

struct VROShaderVertex {
    vec4 position;
} _vertex;

struct VROTransforms {
    mat4 model_matrix;
    mat4 view_matrix;
    mat4 projection_matrix;
} _transforms;

in vec3 position;
in vec3 normal;
in vec2 texcoord;
in vec4 tangent;
in vec4 bone_weights;
in ivec4 bone_indices;
in vec3 morph_0;
in vec3 morph_1;
in vec3 morph_2;
in vec3 morph_3;
in vec3 morph_4;
in vec3 morph_5;
in vec3 morph_6;

uniform highp vec3 camera_position;
uniform mat4 normal_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

#pragma geometry_modifier_uniforms
#pragma vertex_modifier_uniforms

out mat3 v_tbn;
out vec2 v_texcoord;
out vec3 v_surface_position;
flat out int v_instance_id;

void main() {
#inject vertex_assignments

    _transforms.model_matrix = model_matrix;
    _transforms.view_matrix = view_matrix;
    _transforms.projection_matrix = projection_matrix;

    v_instance_id = gl_InstanceID;

#pragma geometry_modifier_body

    v_texcoord = _geometry.texcoord;
    v_surface_position = (_transforms.model_matrix * vec4(_geometry.position, 1.0)).xyz;

    vec3 n = normalize((normal_matrix * vec4(_geometry.normal, 0.0)).xyz);
    vec3 t = normalize((normal_matrix * vec4(_geometry.tangent.xyz, 0.0)).xyz);
    vec3 b = normalize((normal_matrix * vec4((cross(_geometry.normal, _geometry.tangent.xyz) * _geometry.tangent.w), 0.0)).xyz);
    v_tbn = mat3(t, b, n);

    _vertex.position = _transforms.projection_matrix * _transforms.view_matrix * _transforms.model_matrix * vec4(_geometry.position, 1.0);

#pragma vertex_modifier_body
    
    gl_Position = _vertex.position;
}
