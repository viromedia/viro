//
//  VROAnimationQuaternion.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/28/15.
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
