#version 300 es

in highp vec3 v_world_position;
out highp vec4 frag_color;
uniform samplerCube environment_map;

const highp float PI = 3.14159265359;

void main() {
    highp vec3 N = normalize(v_world_position);
    highp vec3 irradiance = vec3(0.0);
    
    highp vec3 up    = vec3(0.0, 1.0, 0.0);
    highp vec3 right = normalize(cross(up, N));
    up               = cross(N, right);
    
    // This value controls how much we sample the cubemap for convolution.
    // Using 0.05 is too low, causes GPU hang errors on iOS (IOAF code 5)
    highp float sample_delta = 0.125;
    highp float sample_count = 0.0f;
    
    highp vec2 seed = vec2(v_world_position.xy);
    
    // Iterate across the sphere: in this shader we are discretely sampling the
    // cubemap using a hemisphere of normals emanating around N (which is the center
    // normal *and* the position on the resultant irradiance cubemap that we are
    // now rendering).
    
    for (highp float phi = 0.0; phi < 2.0 * PI; phi += sample_delta) {
        for (highp float theta = 0.0; theta < 0.5 * PI; theta += sample_delta) {
            
            // Derive the light vector in tangent space (e.g. assuming N was (0, 1, 0)) and
            // we were sampling a hemisphere with Y up
            highp vec3 tangent_sample_vec = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            
            // Convert the light vector to world space (e.g. now we're sampling the hemisphere
            // centered in the direction of N)
            highp vec3 world_sample_vec = tangent_sample_vec.x * right + tangent_sample_vec.y * up + tangent_sample_vec.z * N;
            
            // Sample the environment map with the light vector; modulate by sin(theta) to account
            // for smaller sampling area in the high hemisphere, and modulate by cos(theta) to
            // account for light being weaker at large angles
            irradiance += texture(environment_map, world_sample_vec).rgb * cos(theta) * sin(theta);
            sample_count++;
        }
    }
    
    irradiance = PI * irradiance * (1.0 / sample_count);
    frag_color = vec4(irradiance, 1.0);
}
