//
//  VROAnimationFloat.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/17/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROAnimationFloat_h
#define VROAnimationFloat_h

#include "VROAnimation.h"
#include "VROAnimatable.h"
#include "VROMath.h"

class VROAnimationFloat : public VROAnimation {
    
public:
    
    VROAnimationFloat(std::function<void(VROAnimatable *const, float)> method,
                      float start, float end) :
        VROAnimation(),
        _keyTimes({ 0, 1 }),
        _keyValues({ start, end }),
        _method(method)
    {}
    
    VROAnimationFloat(std::function<void(VROAnimatable *const, float)> method,
                      float start,
                      float end,
                      std::function<void(VROAnimatable *const)> finishCallback) :
        VROAnimation(finishCallback),
        _keyTimes({ 0, 1 }),
        _keyValues({ start, end }),
        _method(method)
    {}
    
    VROAnimationFloat(std::function<void(VROAnimatable *const, float)> method,
                      std::vector<float> keyTimes, std::vector<float> keyValues) :
        VROAnimation(),
        _keyTimes(keyTimes),
        _keyValues(keyValues),
        _method(method)
    {}
    
    VROAnimationFloat(std::function<void(VROAnimatable *const, float)> method,
                      std::vector<float> keyTimes,
                      std::vector<float> keyValues,
                      std::function<void(VROAnimatable *const)> finishCallback) :
        VROAnimation(finishCallback),
        _keyTimes(keyTimes),
        _keyValues(keyValues),
        _method(method)
    {}
    
    void processAnimationFrame(float t) {
        float value = VROMathInterpolateKeyFrame(t, _keyTimes, _keyValues);
        
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
    std::vector<float> _keyValues;
    std::function<void(VROAnimatable *const, float)>  _method;
    
};

#endif /* VROAnimationFloat_h */
