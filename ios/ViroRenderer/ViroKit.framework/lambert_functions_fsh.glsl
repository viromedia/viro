#include lighting_general_functions_fsh

lowp vec3 apply_light_lambert(const VROLightUniforms light,
                              highp vec3 surface_pos,
                              lowp vec3 surface_normal,
                              lowp vec4 material_color) {
    
    highp vec3 surface_to_light;
    highp float attenuation = compute_attenuation(light, surface_pos, surface_to_light);
    
    highp float diffuse_coeff = max(0.0, dot(-surface_normal, surface_to_light));
    return attenuation * diffuse_coeff * material_color.rgb * light.color;
}

lowp vec4 lambert_lighting_diffuse_fixed(VROSurface surface,
                                         highp vec3 camera_position) {
    lowp vec3 light_ambient_color = ambient_light_color * surface.diffuse_color.xyz;

    lowp vec4 material_diffuse_color = surface.diffuse_color * surface.diffuse_intensity;
    lowp vec3 light_diffuse_color = vec3(0, 0, 0);
    for (int i = 0; i < num_lights; i++) {
        light_diffuse_color += apply_light_lambert(lights[i],
                                                   surface.position,
                                                   surface.normal,
                                                   material_diffuse_color);
    }
    
    return vec4(light_ambient_color + light_diffuse_color,
                surface.alpha * material_diffuse_color.a);
}

lowp vec4 lambert_lighting_diffuse_texture(VROSurface surface,
                                           highp vec3 camera_position,
                                           sampler2D sampler) {
    
    lowp vec4 material_diffuse_color = texture(sampler, surface.diffuse_texcoord) * surface.diffuse_color * surface.diffuse_intensity;
    lowp vec3 light_ambient_color = ambient_light_color * material_diffuse_color.xyz;

    lowp vec3 light_diffuse_color = vec3(0, 0, 0);
    for (int i = 0; i < num_lights; i++) {
        light_diffuse_color += apply_light_lambert(lights[i],
                                                   surface.position,
                                                   surface.normal,
                                                   material_diffuse_color);
    }
    
    return vec4(light_ambient_color + light_diffuse_color,
                surface.alpha * material_diffuse_color.a);
}
