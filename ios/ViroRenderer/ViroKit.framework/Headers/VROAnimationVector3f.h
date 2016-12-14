//
//  VROAnimationVector3f.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/17/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROAnimationVector3f_h
#define VROAnimationVector3f_h

#include <stdio.h>
#include "VROVector3f.h"
#include "VROAnimation.h"
#include "VROAnimatable.h"

class VROAnimationVector3f : public VROAnimation {
    
public:
    
    VROAnimationVector3f(std::function<void(VROAnimatable *const, VROVector3f)> method,
                         VROVector3f start,
                         VROVector3f end) :
        VROAnimation(),
        _start(start),
        _end(end),
        _method(method)
    {}
    
    VROAnimationVector3f(std::function<void(VROAnimatable *const, VROVector3f)> method,
                         VROVector3f start,
                         VROVector3f end,
                         std::function<void(VROAnimatable *const)> finishCallback) :
    VROAnimation(finishCallback),
    _start(start),
    _end(end),
    _method(method)
    {}
    
    void processAnimationFrame(float t) {
        VROVector3f value = _start.interpolate(_end, t);
        
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
    
    VROVector3f _start, _end;
    std::function<void(VROAnimatable *const, VROVector3f)> _method;
    
};

#endif /* VROAnimationVector3f_h */
