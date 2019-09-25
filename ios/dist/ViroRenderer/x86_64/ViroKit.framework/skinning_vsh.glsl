layout (std140) uniform bones {
    mat4 bone_matrices[192];
};

layout (std140) uniform bones_dq {
    vec4 bone_transforms_dq[192 * 3];
};

vec4 get_blended_scale(ivec4 bone_indices, vec4 bone_weights) {
    vec4 s0 = bone_transforms_dq[bone_indices.x * 3 + 2];
    vec4 s1 = bone_transforms_dq[bone_indices.y * 3 + 2];
    vec4 s2 = bone_transforms_dq[bone_indices.z * 3 + 2];
    vec4 s3 = bone_transforms_dq[bone_indices.w * 3 + 2];
    
    vec4 blended_s = s0 * bone_weights.x +
                     s1 * bone_weights.y +
                     s2 * bone_weights.z +
                     s3 * bone_weights.w;
    return blended_s;
}

mat2x4 get_bone_dual_quaternion(int bone_index) {
    return mat2x4(bone_transforms_dq[bone_index * 3 + 0],
                  bone_transforms_dq[bone_index * 3 + 1]);
}

mat2x4 get_blended_dual_quaternion(ivec4 bone_indices, vec4 bone_weights) {
    mat2x4 dq0 = get_bone_dual_quaternion(bone_indices.x);
    mat2x4 dq1 = get_bone_dual_quaternion(bone_indices.y);
    mat2x4 dq2 = get_bone_dual_quaternion(bone_indices.z);
    mat2x4 dq3 = get_bone_dual_quaternion(bone_indices.w);
    
    if (dot(dq0[0], dq1[0]) < 0.0) bone_weights.y *= -1.0;
    if (dot(dq0[0], dq2[0]) < 0.0) bone_weights.z *= -1.0;
    if (dot(dq0[0], dq3[0]) < 0.0) bone_weights.w *= -1.0;
    
    mat2x4 blended_dq = dq0 * bone_weights.x +
                        dq1 * bone_weights.y +
                        dq2 * bone_weights.z +
                        dq3 * bone_weights.w;
    
    float norm = length(blended_dq[0]);
    return blended_dq / norm;
}

vec3 quat_rotate_vector(vec3 v, vec4 real_dq) {
    return v + 2.0 * cross(real_dq.w * v + cross(v, real_dq.xyz), real_dq.xyz);
}

vec3 dual_quat_transform_point(vec3 p, vec4 real_dq, vec4 dual_dq) {
    // Reconstruct translation from the dual quaternion
    vec3 t = 2.0 * (real_dq.w * dual_dq.xyz - dual_dq.w * real_dq.xyz + cross(real_dq.xyz, dual_dq.xyz));

    // Combine with rotation of the input point
    return quat_rotate_vector(p, real_dq) + t;
}
