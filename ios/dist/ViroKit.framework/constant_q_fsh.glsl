#version 300 es
#include constant_functions_fsh

uniform lowp vec4 material_diffuse_surface_color;
uniform lowp float material_diffuse_intensity;
uniform lowp float material_alpha;

uniform samplerCube diffuse_texture;

in lowp vec3 v_normal;
in highp vec2 v_texcoord;
in highp vec3 v_surface_position;

out lowp vec4 frag_color;

void main() {
    highp vec3 texcoord = vec3(v_surface_position.x, v_surface_position.y, -v_surface_position.z);
    frag_color = material_diffuse_surface_color * texture(diffuse_texture, texcoord) * material_diffuse_intensity * vec4(1.0, 1.0, 1.0, material_alpha);
}
