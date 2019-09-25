#version 300 es
#include constant_functions_fsh_adreno

uniform lowp vec4 material_diffuse_surface_color;
uniform lowp float material_diffuse_intensity;
uniform lowp float material_alpha;

uniform samplerCube diffuse_texture;

#pragma surface_modifier_uniforms
#pragma fragment_modifier_uniforms

in lowp mat3 v_tbn;
in highp vec2 v_texcoord;
in highp vec3 v_surface_position;

layout (location = 0) out highp vec4 frag_color;

void main() {
    _surface_diffuse_color = material_diffuse_surface_color;
    _surface_diffuse_texcoord = v_texcoord;
    _surface_diffuse_intensity = material_diffuse_intensity;
    _surface_alpha = material_alpha;
    _surface_normal = v_tbn[2];
    _surface_position = v_surface_position;

#pragma surface_modifier_body

    highp vec3 texcoord = vec3(_surface_position.x, _surface_position.y, -_surface_position.z);
    highp vec4 _output_color = _surface_diffuse_color * texture(diffuse_texture, texcoord) *
                              _surface_diffuse_intensity * vec4(1.0, 1.0, 1.0, _surface_alpha);
    
#pragma fragment_modifier_body
    
    frag_color = _output_color;
}
