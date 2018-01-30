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
