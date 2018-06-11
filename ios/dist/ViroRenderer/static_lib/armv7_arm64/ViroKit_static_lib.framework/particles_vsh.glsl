// Grouped in 4N slots, should match VROParticlesUBOVertexData structure defined in VROParticleUBO.h
layout(std140) uniform particles_vertex_data{
   mat4 particles_vertex_transform[180];
};