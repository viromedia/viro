//
//  VROMorpher.h
//  ViroRenderer
//
//  Copyright © 2018 Viro Media. All rights reserved.
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

#ifndef VROMorpher_h
#define VROMorpher_h

#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <memory>
#include <map>
#include <set>
#include "VROVector4f.h"
#include "VROGeometrySource.h"
#include "VROShaderProgram.h"
#include "VROAnimatable.h"

class VROGeometrySource;
class VROgeometry;
class VROMaterial;

/*
 VROMorphTarget contains geometric source data and morph-specific properties, that when applied
 on top of a base geometry in VROMorpher, produces a "blended / morphed" geo-source mesh that
 represents the visual outcome of this target.
 */
struct VROMorphTarget {
    // True if this morph target's animation should be computed on the CPU.
    bool isCPU;

    // Start and end morph weight values used for morphing animations.
    float startWeight;
    float endWeight;

    // False if we wish to ignore this VROMorphTarget from all geometric calculations done in VROMorpher.
    bool isActive;

    // Map of each VROGeometrySemantic (Norm/Pos/Tangent) to its corresponding
    // raw VROGeometrySource vertex data.
    std::map<VROGeometrySourceSemantic, std::shared_ptr<VROGeometrySource>> geometrySources;

    // Map of each VROGeometrySourceSemantic (Norm/Pos/Tangent) to its corresponding
    // vertex data as an array of VROVector4f objects.
    std::map<VROGeometrySourceSemantic, std::vector<VROVector4f>> geometryVecs;
};

/*
 VROMorpher blends VROMorphTargets and applies the blend to a set of base VROGeometrySources.
 The base sources represents the original mesh of the 3D model we are morphing.
 */
class VROMorpher : public VROAnimatable {
public:
    /*
     ComputeLocation represents the method used to calculate and blend VROMorphTarget data.
     */
    enum class ComputeLocation {
        /*
         Performs morph target blending calculations all on the CPU and uploads
         the net deformed base geometric result into the VBOs through VROGeometrySources.

         Animating morph weights are interpolated on the CPU and applied manually into the
         net deformed base geometric result.
         */
        CPU,

        /*
         Performs morph target blending calculations on the GPU. This is done by plumbing
         each target's data through vertex attributes and blending them in the vertex shader.
         Because the number of Vertex Attributes are limited, we only currently support at
         most 7 morph target properties with this mode.

         Animating morph weights are interpolated on the CPU and updated through uniforms.
         */
        GPU,

        /*
         Performs morph target blending calculations on the CPU and uploads only the
         net morph target result into vertex attributes. These are later then blended
         with the base geometric source on the GPU side in the vertex shader.

         Animated morph weights are interpolated on the GPU instead. This mode is optimized
         for animating large amounts of morph target properties, infrequently.
         */
        Hybrid
    };

    VROMorpher(std::vector<std::shared_ptr<VROGeometrySource>> &baseSources, std::shared_ptr<VROMaterial> mat);
    virtual ~VROMorpher();

    /*
     Adds a VROMorphTarget to this VROMorpher, based on the given vec of geometric source and name.
     */
    void addTarget(std::vector<std::shared_ptr<VROGeometrySource>> morphSources,
                   std::string name,
                   float initialWeight);

    /*
     Returns the list of names representing all VROMorphTargets set on this VROMorpher.
     */
    const std::set<std::string> getMorphTargetKeys();

    /*
     Returns true if the given RenderMode is successfully set on this VROMorpher.
     */
    bool setComputeLocation(VROMorpher::ComputeLocation enabled);

    /*
     Returns the currently set RenderMode for this VROMorpher.
     */
    const VROMorpher::ComputeLocation getComputeLocation();

    /*
     Processes all VROMorphTargets associated with this VROMorpher based on the current
     _computeLocation and updates the given vec of geometrySources with processed morph
     target data to be prewarmed and rehydrated for a given geometry.
     */
    bool update(std::vector<std::shared_ptr<VROGeometrySource>> &geometrySources);

    /*
     Sets the weight of a VROMorphTarget in this VROMorpher, matching the given key.
     */
    void setWeightForTarget(std::string key, float weight, bool animate = true);
private:

    /*
     The base set of geometry upon which to apply additional weighted VROMorphTargets.
     */
    std::shared_ptr<VROMorphTarget> _baseTarget;

    /*
     Weighted Morph targets used for morphing / blending the _baseTarget of this VROMorpher.
     */
    std::map<std::string, std::shared_ptr<VROMorphTarget>> _morphTargets;

    /*
     The current rendering mode used to update and render VROMorphTarget data
     for this VROMorpher.
     */
    ComputeLocation _computeLocation;

    /*
     True if this VROMorpher requires a prewarm and hydration update for the base VROGeometry
     that it is applied on.
     */
    bool _needsUpdate;

    /*
     Materials and shader modifiers used to process VROMorphTarget data in vertex shaders.
     */
    std::shared_ptr<VROMaterial> _material;
    std::shared_ptr<VROShaderModifier> _shaderMod;

    /*
     The element index identifying the GeometrySources on a VROGeometry this VROMorpher
     will process.
     */
    int _geometryElementIndex;

    /*
     Used for animating VROMorphTargets in RenderMode::Hybrid, where animated morph weights
     are interpolated on the GPU based on a normalized _hybridAnimationDuration as its T value.
     */
    float _hybridAnimationDuration;

    /*
     Performs blending calculations for all VROMorphTargets associated with this VROMorpher and
     consolidates the results in VROGeometrySource sourcesOut. If isBaseAttribute is true,
     base VROGeometrySource elements are created. Else VROGeometrySource::Morph elements are
     created (for Hybrid modes).
     */
    void processMorphTargets(bool isBaseAttribute,
                             std::vector<std::shared_ptr<VROGeometrySource>> &sourcesOut);

    /*
     Converts all VROMorphTarget data associated with this VROMorpher into the current
     _computeLocation (GPU, CPU or Hybrid) and reconfigures shaders to take advantage of this mode.
     */
    void configureMorphTargets();
    void configureShadersHybrid();
    void configureShadersGPU();

    /*
     Coverts a given VROMorphTarget data into a CPU or GPU type, usually called when
     a new _computeLocation is set.
     */
    std::shared_ptr<VROMorphTarget> convertMorphTargetToCPU(std::shared_ptr<VROMorphTarget> targetIn);
    std::shared_ptr<VROMorphTarget> convertMorphTargetToGPU( std::shared_ptr<VROMorphTarget> targetIn);
    std::shared_ptr<VROGeometrySource> convertVecToGeoSource(std::vector<VROVector4f> &dataVecIn,
                                                             VROGeometrySourceSemantic semantic);

    /*
     Shader semantics mapping OpenGL's Attribute indexes to VROMorph indexes.
     */
    static VROGeometrySourceSemantic getMorphSemanticAttrIndex(int i);
    static VROShaderMask getMorphShaderAttrIndex(int i);

    /*
     Appends to the given vertex shader code additional modifiers. This is used when configuring
     shaders for GPU Morph Targets.
     */
    static void addMorphModifier(int morphTargetIndex, std::string &shaderCode, bool isVec4);

    /*
     Helper functions for adding weighted morph target data to an existing float srcDataOut array
     representing the base geometric source. NOTE: These functions are heavily, heavily used
     and are considered performance sensitive functions!
     */
    inline void addWeightedMorphToSrc3(float *srcDataOut, std::vector<VROVector4f> &morphData, float weight);
    inline void addWeightedMorphToSrc4(float *srcDataOut, std::vector<VROVector4f> &morphData, float weight);

    /*
     Helper functions for resetting an existing float srcDataOut array to 0. NOTE: These functions
     are heavily, heavily used and are considered performance sensitive functions!
     */
    inline void resetSrc3(float *srcDataOut, int size);
    inline void resetSrc4(float *srcDataOut, int size);
};

#endif /* VROMorpher_h */
