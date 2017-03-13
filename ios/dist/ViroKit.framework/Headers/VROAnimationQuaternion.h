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

class VROAnimationQuaternion : public VROAnimation {
    
public:
    
    VROAnimationQuaternion(std::function<void(VROAnimatable *const, VROQuaternion)> method,
                           VROQuaternion start,
                           VROQuaternion end) :
        VROAnimation(),
        _start(start),
        _end(end),
        _method(method)
    {}
    
    VROAnimationQuaternion(std::function<void(VROAnimatable *const, VROQuaternion)> method,
                           VROQuaternion start,
                           VROQuaternion end,
                           std::function<void(VROAnimatable *const)> finishCallback) :
    VROAnimation(finishCallback),
    _start(start),
    _end(end),
    _method(method)
    {}
    
    void processAnimationFrame(float t) {
        VROQuaternion value = VROQuaternion::slerp(_start, _end, t);
        
        std::shared_ptr<VROAnimatable> animatable = _animatable.lock();
        if (animatable) {
            _method(animatable.get(), value);
        }
    }
    
    void finish() {
        std::shared_ptr<VROAnimatable> animatable = _animatable.lock();
        if (animatable) {
            _method(animatable.get(), _end);
        }
    }
    
private:
    
    VROQuaternion _start, _end;
    std::function<void(VROAnimatable *const, VROQuaternion)> _method;
    
};

#endif /* VROAnimationQuaternion_h */
