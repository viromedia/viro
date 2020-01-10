//
//  VROSkinner.h
//  ViroRenderer
//
//  Created by Raj Advani on 5/9/17.
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

#ifndef VROSkinner_h
#define VROSkinner_h

#include "VROMatrix4f.h"
#include "VROGeometrySource.h"
#include <vector>
#include <memory>

class VRONode;
class VROGeometry;
class VROSkeleton;
class VROBone;
/*
 VROSkinner is the base class for skeletal animation; it associates an animation
 skeleton with the geometry that will be deformed.
 
 A single VROSkeleton can be used by multiple VROGeometries; each geometry using
 the VROSkeleton will have its own VROSkinner that maps the geometry to the 
 skeleton.
 
 Brief explanation of coordinate transformations
 ------
 
 Skeletal animation works by associating a geometry with a skeleton. The geometry
 is transformed first so that it aligns with the skeleton, via the _bindTransforms.
 Next, we associate each vertex in the geometry with the set of bones that will
 'influence' it, via _boneWeights and _boneIndices. Then we animate the
 skeleton, specifically by animating each of the bone's transforms. And finally, we 
 deform the mesh to follow the movement of the skeleton.
 
 The steps, in more detail:
 
 1. Bind the geometry to the skeleton.
 
 The position in which our model is encoded in its vertex array is called its
 'original position'. Initially, our model is in its original position, in model
 space. For each vertex we animate, we need to transform it from its original 
 position, model space, to the bind position for the bone that's influencing it,
 in bone local space. Note we do this for every bone that influences the vertex.
 This is the purpose of the geometryBindTransform and the bindTransform for each bone:
 
 Model space, original position  --> [bindTransform] --> Bone space, bind position
 
 2. Animate the skeleton locally
 
 Once we have a vertex in the bind position, bone local space, it can follow the
 animations of the skeleton. To animate, we multiply by the boneTransform. The 
 boneTransform (retrieved via skeleton->getTransform(i) for bone 'i'), transforms
 from the bind position, bone local space to the animated position. The coordinate
 space of the animated position depends on the type of boneTransform used:
 
 [Concatenated bone transform] Bone space, bind position --> [boneTransform] --> Model space, animated position
 [Local bone transform]        Bone space, bind position --> [boneTransform] --> Parent bone space, animated position
 [Legacy bone transform]       Bone space, bind position --> [boneTransform] --> Bone space, animated position
 
 3. Return to model space
 
 If we were using legacy transforms, then we have to return to the model space of the
 geometry with a final transform:
 
 Bone space, animated position  --> [inverseBindTransform] --> Model space, animated position
 
 All transforms listed above are concatenated together by VROSkinner::getModelTransform.
 
 4. Deform the mesh

 All the final bone transforms are then written to the vertex shader via the VROBoneUBO.
 For each vertex of the geometry, we deform by all connected bone transforms. These
 connections are determined by the _boneWeights and _boneIndices. The latter indexes
 into the correct bone transform; the former determines how much influence said bone has
 on the vertex.
 */
class VROSkinner {
    
public:
    
    /*
     The geometryBindTransform passed in here transforms from the geometry's 
     original encoded position, in model space, to the bind position in model space.
     
     The boneSpaceTransforms move from the bind position in model space to the bind
     position in bone local space, for each bone. We use these two parameters to 
     construct the _bindTransforms and _inverseBindTransforms fields, then discard
     them.
     */
    VROSkinner(std::shared_ptr<VROSkeleton> skeleton,
               VROMatrix4f geometryBindTransform,
               std::vector<VROMatrix4f> boneSpaceTransforms,
               std::shared_ptr<VROGeometrySource> boneIndices,
               std::shared_ptr<VROGeometrySource> boneWeights);
    virtual ~VROSkinner() {}
    
    /*
     Get the transform that will transform a vertex tied to the given bone from its original
     (encoded) position in model space, to its animated position in model space.
     */
    VROMatrix4f getModelTransform(int boneIndex);
    
    std::shared_ptr<VROSkeleton> getSkeleton() {
        return _skeleton;
    }
    
    const std::shared_ptr<VROGeometrySource> getBoneIndices() const {
        return _boneIndices;
    }
    const std::shared_ptr<VROGeometrySource> getBoneWeights() const {
        return _boneWeights;
    }

    /*
     Get the bindTransforms that have been set on this VROSkinner, with data that
     corresponds to the order of _boneIndices.
     */
    const std::vector<VROMatrix4f> &getBindTransforms() const {
        return _bindTransforms;
    }
    const std::vector<VROMatrix4f> &getInverseBindTransforms() const {
        return _inverseBindTransforms;
    }

    /*
     Set and grabs the VRONode containing this skinner, if any.
     */
    void setSkinnerNode(std::shared_ptr<VRONode> node) {
        _skinnerNodeWeak = node;
    }
    std::shared_ptr<VRONode> getSkinnerNode() {
        return _skinnerNodeWeak.lock();
    }
    
private:
    
    /*
     The skeleton that drives the animation. The skeleton is *also* in model space.
     */
    std::shared_ptr<VROSkeleton> _skeleton;
    
    /*
     These transforms move vertices from from their original position in model space,
     to the bind position in the local space of the bone at index 'i'.
     
     The purpose of these transforms is two-fold:
     
     1. To place the geometry into the bind pose, the pose at which the geometry
     aligns with the skeleton, and from which its vertices can therefore be animated
     alongside the skeleton's bones.
     
     2. To transform from model space into the coordinate system of a given bone. 
     This way we can animate vertices hierarchically: e.g. a finger vertex can 
     animate around the finger bone, then the elbow, then the shoulder, etc. See
     VROBone.h and getModelTransform() for more details.
     
     Finally, note that the transform at _bindTransforms[i] is for the bone in
     _skeleton.bones[i].
     
     The inverse bindTransforms go the other direction, from bone local 
     space back to model space.
     */
    std::vector<VROMatrix4f> _bindTransforms;
    std::vector<VROMatrix4f> _inverseBindTransforms;
    
    /*
     Vertex data that maps each vertex to the bones that influence its position
     during skeletal animation. The indices map into the _skeleton's _bones array.
     */
    std::shared_ptr<VROGeometrySource> _boneIndices;
    
    /*
     Vertex data that for each vertex defines the weight each bone (mapped to
     via _boneIndices) has in influencing the vertex.
     */
    std::shared_ptr<VROGeometrySource> _boneWeights;

    /*
     Weak pointer to the node containing this VROSKinner.
     */
    std::weak_ptr<VRONode> _skinnerNodeWeak;
};

#endif /* VROSkinner_h */
