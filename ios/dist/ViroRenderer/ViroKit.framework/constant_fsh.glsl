#version 300 es
#include constant_functions_fsh

uniform highp vec4 material_diffuse_surface_color;
uniform highp float material_diffuse_intensity;
uniform lowp float material_alpha;

#pragma surface_modifier_uniforms
#pragma fragment_modifier_uniforms

flat in int v_instance_id;
in lowp mat3 v_tbn;
in highp vec2 v_texcoord;
in highp vec3 v_surface_position;

layout (location = 0) out highp vec4 frag_color;

void main() {
    _surface.diffuse_color = material_diffuse_surface_color;
    _surface.diffuse_texcoord = v_texcoord;
    _surface.diffuse_intensity = material_diffuse_intensity;
    _surface.alpha = material_alpha;
    _surface.normal = v_tbn[2];
    _surface.position = v_surface_position;

#pragma surface_modifier_body

    highp vec4 _output_color = vec4(_surface.diffuse_color.xyz * _surface.diffuse_intensity, _surface.alpha * _surface.diffuse_color.a);
    
#pragma fragment_modifier_body
    
    frag_color = _output_color;
}
