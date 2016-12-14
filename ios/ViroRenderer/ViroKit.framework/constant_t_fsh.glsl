#version 300 es
#include constant_functions_fsh

uniform lowp float material_diffuse_intensity;
uniform lowp float material_alpha;

uniform sampler2D sampler;

in lowp vec3 v_normal;
in highp vec2 v_texcoord;
in highp vec3 v_surface_position;

out lowp vec4 frag_color;

void main() {
    lowp vec4 diffuse_texture_color = texture(sampler, v_texcoord);
    frag_color = vec4(diffuse_texture_color.xyz,
                      material_alpha * diffuse_texture_color.a);
}