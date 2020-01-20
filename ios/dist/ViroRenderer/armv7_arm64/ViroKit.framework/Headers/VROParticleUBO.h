//
//  VROParticleUBO.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef VROParticleUBO_h
#define VROParticleUBO_h

#include "VROInstancedUBO.h"
#include "VROAtomic.h"

static const int kMaxParticlesPerUBO = 180;
static const int kMaxFloatsPerTransform = 16;
static const int kMaxFloatsPerColor = 4;

/*
 Uniform buffer object structure format through which particle transform data are batched
 into the Vertex shader. Data is grouped in 4N slots, matching layout specified
 in particle_vsh.glsl.
 */
typedef struct {
    float particles_transform[kMaxParticlesPerUBO * kMaxFloatsPerTransform];
} VROParticlesUBOVertexData;

/*
 Uniform buffer object structure format through which particle "appearance" data are
 batched into the Fragment shader. Data is grouped in 4N slots, matching layout specified
 in particle_fsh.glsl.
 */
typedef struct {
    float frag_particles_color[kMaxParticlesPerUBO * kMaxFloatsPerColor];
} VROParticlesUBOFragmentData;

class VROParticle;

/*
 VROParticleUBO handles the binding and batching of particle information into the vertex
 and fragment shaders for instance rendering. Note that any instance of a given VROParticleUBO
 will always bind data to the same uniform buffers (Binding Point and Buffer ID).
 */
class VROParticleUBO : public VROInstancedUBO {
public:
    VROParticleUBO(std::shared_ptr<VRODriver> driver);
    virtual ~VROParticleUBO();

    void bind();
    std::vector<std::shared_ptr<VROShaderModifier>> createInstanceShaderModifier();

    /*
     Returns number of glDraw(s) required to instance draw all the particles represented
     by this VROParticleUBO.
     */
    int getNumberOfDrawCalls();

    /*
     Binds all VROParticlesUBOVertexData and VROParticlesUBOFragmentData to its corresponding
     vertex and fragment uniform buffers so that they can be referred to and processed
     by shader modifiers crated with createInstanceShaderModifier().
     */
    int bindDrawData(int currentDrawCallIndex);

    /*
     Update the data in this UBO with the latest list of instanced particle data
     like transformation matrix / color.
     */
    void update(std::vector<VROParticle> &particles, VROBoundingBox &box);

    /*
     Returns a bounding box that encapsulates all _lastKnownParticles.
     */
    VROBoundingBox getInstancedBoundingBox();
    
private:

    /*
     Buffer IDs representing the Uniform Buffer objects that were generated for this
     UBO.
     */
    GLuint _particleVertexUBO;
    GLuint _particleFragmentUBO;

    /*
     The driver that created this UBO.
     */
    std::weak_ptr<VRODriver> _driver;
    
    /*
     The particles and the bounding box after the last update. Note that the bounding
     box is atomic becuase it may be accessed from the application thread (see VRONode's
     application properties).
     */
    std::vector<VROParticle> _lastKnownParticles;
    VROAtomic<VROBoundingBox> _lastKnownBoundingBox;
};

#endif /* VROParticleUBO_h */
