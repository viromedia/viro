#include lighting_general_functions_fsh

struct VROPhongLighting {
    highp vec4 position;
    lowp vec3 normal;
    highp vec2 texcoord;
    
    highp vec3 surface_position;
    highp vec3 camera_position;
    
    lowp vec3 ambient_color;
    lowp vec4 material_color;
    lowp float  material_shininess;
    lowp float  diffuse_intensity;
    lowp float  material_alpha;
};

lowp vec3 apply_light_phong(const VROLightUniforms light,
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
        specular_coeff = pow(max(0.0, dot(surface_to_camera,
                                          reflect(surface_to_light, -surface_normal))),
                             material_shininess);
    }
    
    lowp vec3 specular = specular_coeff * material_specular_color.rgb * light.color;
    return attenuation * (diffuse + specular);
}

lowp vec4 phong_lighting_diffuse_fixed(VROPhongLighting phong,
                                       sampler2D specular_texture) {
    
    lowp vec3 light_ambient_color = phong.ambient_color * phong.material_color.xyz;

    lowp vec4 material_diffuse_color = phong.material_color * phong.diffuse_intensity;
    lowp vec4 material_specular_color = texture(specular_texture, phong.texcoord);
    highp vec3 surface_to_camera = normalize(phong.camera_position - phong.surface_position);
    
    lowp vec3 light_diffuse_color = vec3(0, 0, 0);
    for (int i = 0; i < num_lights; i++) {
        light_diffuse_color += apply_light_phong(lights[i],
                                                 phong.surface_position,
                                                 phong.normal,
                                                 surface_to_camera,
                                                 material_diffuse_color,
                                                 material_specular_color,
                                                 phong.material_shininess);
    }
    
    return vec4(light_ambient_color + light_diffuse_color,
                phong.material_alpha * phong.material_color.a);
}

lowp vec4 phong_lighting_diffuse_texture(VROPhongLighting phong,
                                         sampler2D diffuse_texture,
                                         sampler2D specular_texture) {
    
    lowp vec4 material_diffuse_color = texture(diffuse_texture, phong.texcoord) * phong.material_color * phong.diffuse_intensity;
    lowp vec3 light_ambient_color = phong.ambient_color * material_diffuse_color.xyz;

    lowp vec4 material_specular_color = texture(specular_texture, phong.texcoord);
    highp vec3 surface_to_camera = normalize(phong.camera_position - phong.surface_position);
    
    lowp vec3 light_diffuse_color = vec3(0, 0, 0);
    for (int i = 0; i < num_lights; i++) {
        light_diffuse_color += apply_light_phong(lights[i],
                                                    phong.surface_position,
                                                    phong.normal,
                                                    surface_to_camera,
                                                    material_diffuse_color,
                                                    material_specular_color,
                                                    phong.material_shininess);
    }
    
    return vec4(light_ambient_color + light_diffuse_color,
                phong.material_alpha * material_diffuse_color.a);
}
