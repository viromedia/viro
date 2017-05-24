//
//  VROAnimationQuaternion.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/28/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROAnimationQuaternion_h
#define VROAnimationQuaternion_h

#include <stdio.h>
#include "VROQuaternion.h"
#include "VROAnimation.h"
#include "VROAnimatable.h"
#include "VROMath.h"

class VROAnimationQuaternion : public VROAnimation {
    
public:
    
    VROAnimationQuaternion(std::function<void(VROAnimatable *const, VROQuaternion)> method,
                           VROQuaternion start,
                           VROQuaternion end) :
        VROAnimation(),
        _keyTimes({ 0, 1 }),
        _keyValues({ start, end }),
        _method(method)
    {}
    
    VROAnimationQuaternion(std::function<void(VROAnimatable *const, VROQuaternion)> method,
                           VROQuaternion start,
                           VROQuaternion end,
                           std::function<void(VROAnimatable *const)> finishCallback) :
        VROAnimation(finishCallback),
        _keyTimes({ 0, 1 }),
        _keyValues({ start, end }),
        _method(method)
    {}
    
    VROAnimationQuaternion(std::function<void(VROAnimatable *const, VROQuaternion)> method,
                           std::vector<float> keyTimes,
                           std::vector<VROQuaternion> keyValues) :
        VROAnimation(),
        _keyTimes(keyTimes),
        _keyValues(keyValues),
        _method(method)
    {}
    
    VROAnimationQuaternion(std::function<void(VROAnimatable *const, VROQuaternion)> method,
                           std::vector<float> keyTimes,
                           std::vector<VROQuaternion> keyValues,
                           std::function<void(VROAnimatable *const)> finishCallback) :
        VROAnimation(finishCallback),
        _keyTimes(keyTimes),
        _keyValues(keyValues),
        _method(method)
    {}
    
    void processAnimationFrame(float t) {
        VROQuaternion value = VROMathInterpolateKeyFrameQuaternion(t, _keyTimes, _keyValues);
        
        std::shared_ptr<VROAnimatable> animatable = _animatable.lock();
        if (animatable) {
            _method(animatable.get(), value);
        }
    }
    
    void finish() {
        std::shared_ptr<VROAnimatable> animatable = _animatable.lock();
        if (animatable) {
            _method(animatable.get(), _keyValues.back());
        }
    }
    
private:
    
    std::vector<float> _keyTimes;
    std::vector<VROQuaternion> _keyValues;
    std::function<void(VROAnimatable *const, VROQuaternion)> _method;
    
};

#endif /* VROAnimationQuaternion_h */
