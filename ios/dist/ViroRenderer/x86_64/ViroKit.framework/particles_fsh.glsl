// Grouped in 4N slots, should match VROParticlesUBOFragmentData structure defined in VROParticleUBO.h
layout(std140) uniform particles_fragment_data{
   mediump vec4 particles_fragment_color[180];
};