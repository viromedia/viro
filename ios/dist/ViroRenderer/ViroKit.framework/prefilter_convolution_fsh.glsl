#version 300 es

in highp vec3 v_world_position;
out highp vec4 frag_color;
uniform samplerCube environment_map;
uniform highp float material_roughness;

const highp float PI = 3.14159265359;

highp float distribution_ggx(highp vec3 N, highp vec3 H, highp float roughness) {
    highp float a = roughness*roughness;
    highp float a2 = a*a;
    highp float nDotH = max(dot(N, H), 0.0);
    highp float nDotH2 = nDotH*nDotH;

    highp float nom   = a2;
    highp float denom = (nDotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

highp float vanDerCorpus(uint n, uint base) {
    highp float invBase = 1.0 / float(base);
    highp float denom   = 1.0;
    highp float result  = 0.0;

    for(uint i = 0u; i < 32u; ++i) {
        if(n > 0u) {
            denom   = mod(float(n), 2.0);
            result += denom * invBase;
            invBase = invBase / 2.0;
            n       = uint(float(n) / 2.0);
        }
    }

    return result;
}

highp vec2 hammersley_no_bitOps(uint i, uint N) {
    return vec2(float(i)/float(N), vanDerCorpus(i, 2u));
}

highp vec3 importance_sample_ggx(highp vec2 Xi, highp vec3 N, highp float roughness) {
    highp float a = roughness*roughness;

    highp float phi = 2.0 * PI * Xi.x;
    highp float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    highp float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

    // from spherical coordinates to cartesian coordinates - halfway vector
    highp vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    // from tangent-space H vector to world-space sample vector
    highp vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    highp vec3 tangent   = normalize(cross(up, N));
    highp vec3 bitangent = cross(N, tangent);

    highp vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}

void main() {
    highp vec3 N = normalize(v_world_position);

    // make the simplyfying assumption that V equals R equals the normal
    highp vec3 R = N;
    highp vec3 V = R;

    const uint SAMPLE_COUNT = 256u;
    highp vec3 prefilteredColor = vec3(0.0);
    highp float totalWeight = 0.0;

    for (uint i = 0u; i < SAMPLE_COUNT; ++i) {
        
        // generates a sample vector that's biased towards the preferred alignment direction (importance sampling).
        highp vec2 Xi = hammersley_no_bitOps(i, SAMPLE_COUNT);
        highp vec3 H  = importance_sample_ggx(Xi, N, material_roughness);
        highp vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        highp float NdotL = max(dot(N, L), 0.0);
        if (NdotL > 0.0) {
            // sample from the environment's mip level based on roughness/pdf
            highp float D     = distribution_ggx(N, H, material_roughness);
            highp float nDotH = max(dot(N, H), 0.0);
            highp float hdotV = max(dot(H, V), 0.0);
            highp float pdf   = D * nDotH / (4.0 * hdotV) + 0.0001;

            highp float resolution = 512.0; // resolution of source cubemap (per face)
            highp float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
            highp float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

            highp float mipLevel = material_roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

            prefilteredColor += textureLod(environment_map, L, mipLevel).rgb * NdotL;
            totalWeight      += NdotL;
        }
    }

    prefilteredColor = prefilteredColor / totalWeight;
    frag_color = vec4(prefilteredColor, 1.0);
}
