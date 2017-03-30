#version 300 es
#include lambert_functions_fsh

uniform highp vec3 camera_position;
uniform lowp vec4 material_diffuse_surface_color;
uniform lowp float material_diffuse_intensity;
uniform lowp float material_alpha;

uniform sampler2D diffuse_texture;
uniform samplerCube reflect_texture;

#pragma surface_modifier_uniforms

in lowp vec3 v_normal;
in highp vec2 v_texcoord;
in highp vec3 v_surface_position;

out lowp vec4 frag_color;

void main() {
    _surface.diffuse_color = material_diffuse_surface_color;
    _surface.diffuse_texcoord = v_texcoord;
    _surface.diffuse_intensity = material_diffuse_intensity;
    _surface.alpha = material_alpha;
    _surface.normal = v_normal;
    _surface.position = v_surface_position;
    
#pragma surface_modifier_body
    
    float4 reflective_color = compute_reflection(_surface.position, camera_position, _surface.normal, reflect_texture);
    float4 lighting_color = lambert_lighting_diffuse_texture(_surface, camera_position, diffuse_texture);
    
    frag_color = vec4(lighting_color.xyz + reflective_color.xyz, lighting_color.a);
}
