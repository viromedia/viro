#version 300 es

in highp vec3 v_world_position;
out highp vec4 frag_color;
uniform samplerCube environment_map;

const highp float PI = 3.14159265359;

void main() {
    highp vec3 N = normalize(v_world_position);
    highp vec3 irradiance = vec3(0.0);
    
    highp vec3 up    = vec3(0.0, 1.0, 0.0);
    highp vec3 right = cross(up, N);
    up               = cross(N, right);
    
    // This value controls how much we sample the cubemap for convolution.
    // Using 0.025 is too low, causes GPU hang errors on iOS
    highp float sample_delta = 0.05;
    highp float sample_count = 0.0f;
    
    for (highp float phi = 0.0; phi < 2.0 * PI; phi += sample_delta) {
        for (highp float theta = 0.0; theta < 0.5 * PI; theta += sample_delta) {
            highp vec3 tangent_sample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            highp vec3 sample_vec = tangent_sample.x * right + tangent_sample.y * up + tangent_sample.z * N;
            
            irradiance += texture(environment_map, sample_vec).rgb * cos(theta) * sin(theta);
            sample_count++;
        }
    }
    
    irradiance = PI * irradiance * (1.0 / sample_count);
    frag_color = vec4(irradiance, 1.0);
}
