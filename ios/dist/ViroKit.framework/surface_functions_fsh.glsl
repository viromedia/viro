struct VROSurface {
    lowp  vec4 diffuse_color;
    highp vec2 diffuse_texcoord;
    lowp float diffuse_intensity;
    
    lowp float shininess;
    highp vec2 specular_texcoord;
    
    lowp float alpha;
    lowp  vec3 normal;
    highp vec3 position;
} _surface;
