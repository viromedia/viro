//
//  VROSkeletalAnimationLayer.hpp
//  ViroKit
//
//  Created by Raj Advani on 8/15/18.
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
