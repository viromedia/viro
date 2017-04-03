#include lighting_general_functions_fsh

lowp vec3 apply_light_blinn(const VROLightUniforms light,
                            highp vec3 surface_pos,
                            lowp vec3 surface_normal,
                            highp vec3 surface_to_camera,
                            lowp vec4 material_diffuse_color,
                            lowp vec4 material_specular_color,
                            lowp float  material_shininess) {
    
    highp vec3 surface_to_light;
    highp float attenuation = compute_attenuation(light, surface_pos, surface_to_light);
    
    // Diffuse
    highp float diffuse_coeff = max(0.0, dot(-surface_normal, surface_to_light));
    lowp vec3 diffuse = diffuse_coeff * material_diffuse_color.rgb * light.color;
    
    // Specular
    lowp float specular_coeff = 0.0;
    if (diffuse_coeff > 0.0) {
        specular_coeff = pow(max(0.0, dot(normalize(-surface_to_camera + surface_to_light),
                                          -surface_normal)),
                             material_shininess);
    }
    
    lowp vec3 specular = specular_coeff * material_specular_color.rgb * light.color;
    return attenuation * (diffuse + specular);
}

lowp vec4 blinn_lighting(VROSurface surface,
                         highp vec3 camera_position,
                         sampler2D specular_texture) {

    lowp vec3 light_ambient_color = ambient_light_color.xyz * surface.diffuse_color.xyz;
    
    lowp vec4 material_diffuse_color  = surface.diffuse_color * surface.diffuse_intensity;
    lowp vec4 material_specular_color = texture(specular_texture, surface.specular_texcoord);
    highp vec3 surface_to_camera = normalize(camera_position - surface.position);
    
    lowp vec3 light_diffuse_color = vec3(0, 0, 0);
    for (int i = 0; i < num_lights; i++) {
        light_diffuse_color += apply_light_blinn(lights[i],
                                                 surface.position,
                                                 surface.normal,
                                                 surface_to_camera,
                                                 material_diffuse_color,
                                                 material_specular_color,
                                                 surface.shininess);
    }
    
    return vec4(light_ambient_color + light_diffuse_color,
                surface.alpha * material_diffuse_color.a);
}
