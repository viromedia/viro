//
//  VROSkeletalAnimationLayer.hpp
//  ViroKit
//
//  Created by Raj Advani on 8/15/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROSkeletalAnimationLayer_hpp
#define VROSkeletalAnimationLayer_hpp

#include <map>

class VROSkeletalAnimationLayerInternal;

/*
 A single layer of a VROLayeredSkeletalAnimation. Each layer is comprised of a skeletal
 animation and the properties that define how it blends with the other skeletal animations.
 */
class VROSkeletalAnimationLayer {
    
    friend class VRONode;
    friend class VROLayeredSkeletalAnimation;
    
public:
    VROSkeletalAnimationLayer(std::string name, float defaultBoneWeight) :
        name(name), defaultBoneWeight(defaultBoneWeight) {
    }
    
    void setBoneWeight(int boneIndex, float weight) {
        boneWeights[boneIndex] = weight;
    }
    void setBoneWeights(std::map<int, float> weights) {
        boneWeights = weights;
    }
    
private:
    std::string name;
    std::shared_ptr<VROExecutableAnimation> animation;
    
    // Default weight for every bone influenced by this animation
    float defaultBoneWeight;
    
    // Overriden specific weights for bones
    std::map<int, float> boneWeights;
    
    // Holds the derived data for the animation, making it possible to cache
    // an animation
    std::map<std::shared_ptr<VROSkinner>, std::shared_ptr<VROSkeletalAnimationLayerInternal>> _internal;
    
    
};

#endif /* VROSkeletalAnimationLayer_hpp */
