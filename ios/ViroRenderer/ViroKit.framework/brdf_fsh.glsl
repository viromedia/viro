#version 300 es

out highp vec4 frag_color;
in highp vec2 v_texcoord;

const highp float PI = 3.14159265359;

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

highp float geometry_schlick_ggx(highp float nDotV, highp float roughness) {
    // note that we use a different k for IBL
    highp float a = roughness;
    highp float k = (a * a) / 2.0;
    highp float nom   = nDotV;
    highp float denom = nDotV * (1.0 - k) + k;
    return nom / denom;
}

highp float geometry_smith(highp vec3 N, highp vec3 V, highp vec3 L, highp float roughness) {
    highp float nDotV = max(dot(N, V), 0.0);
    highp float nDotL = max(dot(N, L), 0.0);
    highp float ggx2 = geometry_schlick_ggx(nDotV, roughness);
    highp float ggx1 = geometry_schlick_ggx(nDotL, roughness);
    return ggx1 * ggx2;
}

highp vec2 integrate_brf(highp float nDotV, highp float roughness) {
    highp vec3 V;
    V.x = sqrt(1.0 - nDotV*nDotV);
    V.y = 0.0;
    V.z = nDotV;

    highp float A = 0.0;
    highp float B = 0.0;
    highp vec3 N = vec3(0.0, 0.0, 1.0);
    const uint SAMPLE_COUNT = 256u;

    for(uint i = 0u; i < SAMPLE_COUNT; ++i) {
        // generates a sample vector that's biased towards the
        // preferred alignment direction (importance sampling).
        highp vec2 Xi = hammersley_no_bitOps(i, SAMPLE_COUNT);
        highp vec3 H  = importance_sample_ggx(Xi, N, roughness);
        highp vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        highp float nDotL = max(L.z, 0.0);
        highp float NdotH = max(H.z, 0.0);
        highp float VdotH = max(dot(V, H), 0.0);

        if(nDotL > 0.0) {
            highp float G     = geometry_smith(N, V, L, roughness);
            highp float G_Vis = (G * VdotH) / (NdotH * nDotV);
            highp float Fc    = pow(1.0 - VdotH, 5.0);

            A += (1.0 - Fc) * G_Vis;
            B += Fc * G_Vis;
        }
    }

    A /= float(SAMPLE_COUNT);
    B /= float(SAMPLE_COUNT);
    return vec2(A, B);
}

void main() {
    highp vec2 integratedBRDF = integrate_brf(v_texcoord.x, v_texcoord.y);
    frag_color = vec4 (integratedBRDF.x, integratedBRDF.y , 0.0, 1.0);
}
