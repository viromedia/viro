struct VROSurface {
    highp vec4 diffuse_color;
    highp vec2 diffuse_texcoord;
    highp float diffuse_intensity;
    
    lowp float shininess;
    lowp  vec3 specular_color;
    highp vec2 specular_texcoord;
    
    highp float roughness;
    highp float roughness_intensity;
    highp float metalness;
    highp float metalness_intensity;
    highp float ao;
    
    lowp float alpha;
    lowp  vec3 normal;
    highp vec3 position;
    highp vec3 view;
} _surface;
