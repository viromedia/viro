#extension GL_OES_EGL_image_external : enable
#extension GL_OES_EGL_image_external_essl3 : enable

#include surface_functions_fsh

const highp float PI = 3.14159265359;

// Grouped in 4N slots (int and float take 1N, vec3 takes 3N, vec4 takes 4N)
struct VROLightUniforms {
    int type;
    highp float attenuation_start_distance;
    highp float attenuation_end_distance;
    highp float attenuation_falloff_exp;
    
    highp vec4 position;
    highp vec4 direction;
    
    highp vec3 color;
    highp float spot_inner_angle;
    
    highp float spot_outer_angle;
    int shadow_map_index;
    lowp float shadow_bias;
    lowp float shadow_opacity;
};

layout (std140) uniform lighting_fragment {
    int num_lights;
    lowp float padding0, padding1, padding2;
    
    highp vec4 ambient_light_color;
    VROLightUniforms lights[8];
};

struct VROLightingContribution {
    highp vec3 ambient;
    highp vec3 diffuse;
    highp vec3 specular;
    highp float visibility; // Fades light, e.g. due to shadows
} _lightingContribution;

struct VROShaderLight {
    highp vec3  color;
    highp vec3  surface_to_light;
    highp float attenuation;
    highp vec3  position;
} _light;

highp float compute_attenuation(const VROLightUniforms light,
                                highp vec3 surface_pos,
                                out highp vec3 surface_to_light) {
    
    highp float attenuation = 1.0;
    
    // Directional light
    if (light.type == 1) {
        surface_to_light = -normalize(light.direction.xyz);
        attenuation = 1.0;
    }
    
    // Omni + Spot lights
    else {
        surface_to_light = normalize(light.position.xyz - surface_pos);
        highp float distance_to_light = length(light.position.xyz - surface_pos);
        highp float d = clamp((distance_to_light - light.attenuation_start_distance) /
                              (light.attenuation_end_distance - light.attenuation_start_distance),
                               0.0, 1.0);
        
        attenuation = 1.0 - pow(d, 1.0 / light.attenuation_falloff_exp);
        
        // Spot light
        if (light.type == 3) {
            highp float light_surface_angle = acos(dot(surface_to_light, -normalize(light.direction.xyz)));
            if (light_surface_angle > light.spot_inner_angle) {
                highp float t = clamp((light_surface_angle - light.spot_inner_angle) / light.spot_outer_angle, 0.0, 1.0);
                attenuation = mix(attenuation, 0.0, t);
            }
        }
    }
    
    return attenuation;
}

lowp vec4 compute_reflection(highp vec3 surface_position, highp vec3 camera_position, lowp vec3 normal,
                             samplerCube reflect_sampler) {
    
    highp vec3 surface_to_camera = normalize(surface_position - camera_position);
    highp vec3 reflect_ray = reflect(surface_to_camera, -normal);
    
    return texture(reflect_sampler, vec3(reflect_ray.xy, -reflect_ray.z));
}
