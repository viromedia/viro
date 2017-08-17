struct VROSurface {
    lowp  vec4 diffuse_color;
    highp vec2 diffuse_texcoord;
    lowp float diffuse_intensity;
    
    lowp float shininess;
    lowp  vec3 specular_color;
    highp vec2 specular_texcoord;
    
    lowp float alpha;
    lowp  vec3 normal;
    highp vec3 position;
    highp vec3 view;
} _surface;
