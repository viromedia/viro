struct VROConstantLighting {
    highp vec4 position;
    lowp vec4 color;
    highp vec2 texcoord;
    
    highp vec3 surface_position;
    
    lowp vec3 ambient_color;
    lowp vec4 material_color;
    lowp float diffuse_intensity;
    lowp float material_alpha;
};