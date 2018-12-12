#version 300 es

layout (std140) uniform lighting_vertex {
    int lv_num_lights;
    lowp float lv_padding0, lv_padding1, lv_padding2;
    
    mat4 shadow_view_matrices[8];
    mat4 shadow_projection_matrices[8];
};

vec3 _geometry_position;
vec3 _geometry_normal;
vec2 _geometry_texcoord;
vec4 _geometry_tangent;
vec4 _geometry_bone_weights;
ivec4 _geometry_bone_indices;

vec4 _vertex_position;

mat4 _transforms_model_matrix;
mat4 _transforms_view_matrix;
mat4 _transforms_projection_matrix;

in vec3 position;
in vec3 normal;
in vec2 texcoord;
in vec4 tangent;
in vec4 bone_weights;
in ivec4 bone_indices;

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
    _geometry_position = position;
    _geometry_normal = normal;
    _geometry_texcoord = texcoord;
    _geometry_tangent = tangent;
    _geometry_bone_weights = bone_weights;
    _geometry_bone_indices = bone_indices;

    _transforms_model_matrix = model_matrix;
    _transforms_view_matrix = view_matrix;
    _transforms_projection_matrix = projection_matrix;

    v_instance_id = gl_InstanceID;

#pragma geometry_modifier_body

    v_texcoord = _geometry_texcoord;
    v_surface_position = (_transforms_model_matrix * vec4(_geometry_position, 1.0)).xyz;

    vec3 n = normalize((normal_matrix * vec4(_geometry_normal,  0.0)).xyz);
    vec3 t = normalize((normal_matrix * vec4(_geometry_tangent.xyz, 0.0)).xyz);
    vec3 b = normalize((normal_matrix * vec4((cross(_geometry_normal, _geometry_tangent.xyz) * _geometry_tangent.w), 0.0)).xyz);
    v_tbn = mat3(t, b, n);

    _vertex_position = _transforms_projection_matrix * _transforms_view_matrix * _transforms_model_matrix * vec4(_geometry_position, 1.0);

#pragma vertex_modifier_body
    
    gl_Position = _vertex_position;
}
