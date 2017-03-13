#version 300 es

struct VROShaderGeometry {
    vec3 position;
    vec3 normal;
    vec2 texcoord;
} _geometry;

in vec3 position;
in vec3 normal;
in vec2 texcoord;

uniform mat4 normal_matrix;
uniform mat4 model_matrix;
uniform mat4 modelview_projection_matrix;

#pragma geometry_modifier_uniforms

out vec3 v_normal;
out vec2 v_texcoord;
out vec3 v_surface_position;

void main() {
    _geometry.position = position;
    _geometry.normal = normal;
    _geometry.texcoord = texcoord;
    
#pragma geometry_modifier_body
    
    v_texcoord = _geometry.texcoord;
    v_surface_position = (model_matrix * vec4(_geometry.position, 1.0)).xyz;
    v_normal = normalize((normal_matrix * vec4(_geometry.normal, 0.0)).xyz);
    
    gl_Position = modelview_projection_matrix * vec4(_geometry.position, 1.0);
}
