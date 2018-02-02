/*
 Implementation for the Cook-Torrance specular BRDF. Consists of a Distribution function, a
 Fresnel equation, and a Geometry function.
 */

/*
 Fresnel equation: describes the ratio of surface reflection at different surface angles.
 */
highp vec3 fresnel_schlick(highp float cosTheta, highp vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

highp vec3 fresnel_schlick_roughness(highp float cosTheta, highp vec3 F0, highp float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

/*
 Trowbridge-Reitz GGX normal distribution function: approximates the amount the surface's
 microfacets are aligned to the halfway vector influenced by the roughness of the surface.
 Primary function approximating microfacets.
 */
highp float distribution_ggx(highp vec3 N, highp vec3 H, highp float roughness) {
    highp float a      = roughness * roughness;
    highp float a2     = a * a;
    highp float NdotH  = max(dot(N, H), 0.0);
    highp float NdotH2 = NdotH * NdotH;
    
    highp float nom   = a2;
    highp float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    
    return nom / denom;
}

/*
 Schlick GGX geometry function: describes the self-shadowing property of the microfacets. When
 a surface is relatively rough the surface's microfacets can overshadow other microfacets, reducing
 the light the surface reflects.
 */
highp float geometry_schlick_ggx(highp float NdotV, highp float roughness) {
    highp float r = (roughness + 1.0);
    highp float k = (r * r) / 8.0;
    highp float nom   = NdotV;
    highp float denom = NdotV * (1.0 - k) + k;
    
    return nom / denom;
}

/*
 Smith method combines the obstruction from the view direction with the shadowing from the light
 direction vector. Each is computed with Schlick and combined through Smith.
 */
highp float geometry_smith(highp vec3 N, highp vec3 V, highp vec3 L, highp float roughness) {
    highp float NdotV = max(dot(N, V), 0.0);
    highp float NdotL = max(dot(N, L), 0.0);
    highp float ggx2  = geometry_schlick_ggx(NdotV, roughness);
    highp float ggx1  = geometry_schlick_ggx(NdotL, roughness);
    
    return ggx1 * ggx2;
}

highp float smooth_distance_attenuation(highp float squared_distance, highp float inv_squared_attenuation_radius) {
    highp float factor = squared_distance * inv_squared_attenuation_radius;
    highp float smooth_factor = clamp(1.0f - factor * factor, 0.0, 1.0);
    return smooth_factor * smooth_factor;
}

highp float compute_distance_attenuation(highp vec3 unnormalized_light_vector, highp float inv_squared_attenuation_radius) {
    highp float squared_distance = dot(unnormalized_light_vector, unnormalized_light_vector);
    highp float attenuation = 1.0 / (max(squared_distance, 0.01 * 0.01));
    attenuation *= smooth_distance_attenuation(squared_distance, inv_squared_attenuation_radius);
    
    return attenuation;
}

highp float compute_attenuation_pbr(const VROLightUniforms light, highp vec3 surface_pos,
                                    out highp vec3 L) {
    
    highp float attenuation = 1.0;
    
    // Directional light
    if (light.type == 1) {
        L = -normalize(light.direction.xyz);
        
        // For directional light luminous intensity is simply a scalar (as if we're
        // using Lambert)
        highp float luminous_intensity = light.intensity / (1000.0);
        attenuation = luminous_intensity;
    }
    
    // Omni Light
    else if (light.type == 2) {
        highp vec3 unnormalized_light_vector = light.position.xyz - surface_pos;
        L = normalize(unnormalized_light_vector);
        
        // Intensity is integrated over the solid angle to get power (luminous flux)
        // For a point light this simplifies to I = phi / (4.0 * PI). We use PI
        // becuase we don't want spotlight intensity to differ from omni intensity
        highp float luminous_intensity = light.intensity / PI;

        highp float inverse_square_attenuation_radius = 1.0 / (light.attenuation_end_distance * light.attenuation_end_distance);
        attenuation *= compute_distance_attenuation(unnormalized_light_vector, inverse_square_attenuation_radius);
        attenuation *= luminous_intensity;
    }
    
    // Spotlight
    else if (light.type == 3) {
        highp vec3 unnormalized_light_vector = light.position.xyz - surface_pos;
        L = normalize(unnormalized_light_vector);
        
        // For a spotlight we use a simple equation for intensity since we don't want
        // intensity to be stronger for smaller cones given the same power (luminous flux)
        highp float luminous_intensity = light.intensity / PI;
        
        highp float inverse_square_attenuation_radius = 1.0 / (light.attenuation_end_distance * light.attenuation_end_distance);
        attenuation *= compute_distance_attenuation(unnormalized_light_vector, inverse_square_attenuation_radius);
        attenuation *= compute_angle_attenuation(L, light.direction.xyz, light.spot_inner_angle, light.spot_outer_angle);
        attenuation *= luminous_intensity;
    }
    
    return attenuation;
}

