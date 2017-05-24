//
//  VROSkinner.h
//  ViroRenderer
//
//  Created by Raj Advani on 5/9/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROSkinner_h
#define VROSkinner_h

#include "VROMatrix4f.h"
#include "VROGeometrySource.h"
#include <vector>
#include <memory>

class VROGeometry;
class VROSkeleton;

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
 from the bind position, bone local space to the animated position, bone local
 space.
 
 Bone space, bind position --> [boneTransform] --> Bone space, animated position
 
 3. Return to model space
 
 We have to return to the model space of the geometry with a final transform. These
 transforms are concatenated together by VROSkinner::getModelTransform.
 
 Bone space, animated position  --> [inverseBindTransform] --> Model space, animated position
 
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
     original encoded position, in model space, to the bind position in world space.
     The boneSpaceTransforms move from the bind position in world space, to the bind
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
     Get the concatenated transform that will transform a vertex tied to the
     given bone from its original (encoded) position in model space, to its animated
     position in model space.
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
    
private:
    
    /*
     The geometry being animated, with coordinates in model space.
     */
    std::shared_ptr<VROGeometry> _geometry;
    
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
    
};

#endif /* VROSkinner_h */
