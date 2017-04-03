#version 300 es

struct VROShaderGeometry {
    vec3 position;
    vec3 normal;
    vec2 texcoord;
    vec4 tangent;
} _geometry;

in vec3 position;
in vec3 normal;
in vec2 texcoord;
in vec4 tangent;

uniform mat4 normal_matrix;
uniform mat4 model_matrix;
uniform mat4 modelview_projection_matrix;

#pragma geometry_modifier_uniforms

out mat3 v_tbn;
out vec2 v_texcoord;
out vec3 v_surface_position;

void main() {
    _geometry.position = position;
    _geometry.normal = normal;
    _geometry.texcoord = texcoord;
    _geometry.tangent = tangent;
    
#pragma geometry_modifier_body
    
    v_texcoord = _geometry.texcoord;
    v_surface_position = (model_matrix * vec4(_geometry.position, 1.0)).xyz;
    
    vec3 n = normalize((normal_matrix * vec4(_geometry.normal,  0.0)).xyz);
    vec3 t = normalize((normal_matrix * vec4(_geometry.tangent.xyz, 0.0)).xyz);
    vec3 b = normalize((normal_matrix * vec4((cross(_geometry.normal, _geometry.tangent.xyz) * _geometry.tangent.w), 0.0)).xyz);
    v_tbn = mat3(t, b, n);
    
    gl_Position = modelview_projection_matrix * vec4(_geometry.position, 1.0);
}
