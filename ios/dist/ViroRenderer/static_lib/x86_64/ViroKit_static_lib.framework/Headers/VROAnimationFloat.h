//
//  VROAnimationFloat.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/17/15.
//  Copyright © 2015 Viro Media. All rights reserved.
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
