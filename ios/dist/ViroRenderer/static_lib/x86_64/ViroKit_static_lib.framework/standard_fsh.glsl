#version 300 es
#include lighting_general_functions_fsh
#include pbr_fsh

uniform highp vec3 camera_position;
uniform highp vec4 material_diffuse_surface_color;
uniform highp float material_diffuse_intensity;
uniform lowp float material_alpha;
uniform lowp float material_shininess;
uniform highp float material_roughness;
uniform highp float material_roughness_intensity;
uniform highp float material_metalness;
uniform highp float material_metalness_intensity;
uniform highp float material_ao;

#pragma surface_modifier_uniforms
#pragma fragment_modifier_uniforms
#pragma lighting_model_modifier_uniforms

in lowp mat3 v_tbn;
in highp vec2 v_texcoord;
in highp vec3 v_surface_position;

layout (location = 0) out highp vec4 frag_color;

void main() {
    _surface.diffuse_color = material_diffuse_surface_color;
    _surface.diffuse_texcoord = v_texcoord;
    _surface.diffuse_intensity = material_diffuse_intensity;
    _surface.shininess = material_shininess;
    _surface.specular_color = vec3(0.0, 0.0, 0.0);
    _surface.specular_texcoord = v_texcoord;
    _surface.roughness = max(material_roughness, 0.06); // Clamp to avoid potential divide by 0 (0.06 is min required for iPhone 7)
    _surface.roughness_intensity = material_roughness_intensity;
    _surface.metalness = material_metalness;
    _surface.metalness_intensity = material_metalness_intensity;
    _surface.ao = material_ao;
    _surface.alpha = material_alpha;
    _surface.normal = normalize(v_tbn[2]);
    _surface.position = v_surface_position;
    _surface.view = normalize(camera_position - _surface.position);
    
    highp vec3 _ambient  = ambient_light_color.xyz;
    highp vec3 _diffuse  = vec3(0, 0, 0);
    highp vec3 _specular = vec3(0, 0, 0);
    
#pragma surface_modifier_body
  
    for (int i = 0; i < num_lights; i++) {
        _lightingContribution.ambient  = vec3(0.0);
        _lightingContribution.diffuse  = vec3(0.0);
        _lightingContribution.specular = vec3(0.0);
        _lightingContribution.visibility = 1.0;
        
        VROLightUniforms _light = lights[i];
        
#pragma lighting_model_modifier_body
        
        _ambient  += _lightingContribution.ambient;
        _diffuse  += _lightingContribution.diffuse  * _lightingContribution.visibility;
        _specular += _lightingContribution.specular * _lightingContribution.visibility;
    }
    
    highp vec4 _output_color = vec4(_ambient  * _surface.diffuse_color.xyz +
                                    _diffuse  * _surface.diffuse_color.xyz * _surface.diffuse_intensity +
                                    _specular * _surface.specular_color,
                                    _surface.alpha * _surface.diffuse_color.a);
    
#pragma fragment_modifier_body
    
    frag_color = _output_color;
}

