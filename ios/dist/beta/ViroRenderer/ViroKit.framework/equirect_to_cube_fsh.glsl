#version 300 es

in highp vec3 v_world_position;
out highp vec4 frag_color;
uniform sampler2D equirectangular_map;

const highp vec2 inv_atan = vec2(0.1591, 0.3183);

highp vec2 sample_spherical_map(highp vec3 v) {
    highp vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= inv_atan;
    uv += 0.5;
    return uv;
}

void main() {
    highp vec2 uv = sample_spherical_map(normalize(v_world_position));
    highp vec3 color = texture(equirectangular_map, uv).rgb;
    frag_color = vec4(color, 1.0);
}
