//
//  VROAnimation.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/15/15.
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

#ifndef VROAnimation_h
#define VROAnimation_h

#include <stdio.h>
#include <memory>
#include <functional>
#include "VROAnimatable.h"

/*
 Represents the animation of a property. Subclasses identify the type
 of the property (e.g. animating a floating point value, animating a
 VROVector3f, etc.).
 */
class VROAnimation {
    
public:
    
    VROAnimation() {}
    VROAnimation(std::function<void(VROAnimatable *const)> finishCallback) :
        _finishCallback(finishCallback)
    {}
    
    virtual ~VROAnimation() {}
    
    /*
     Set the animatable. This is typically the object holding the 
     property to be animated. It is stored by a weak_ptr so we can
     ensure it hasn't been deallocated before invoking the animation
     each frame.
     */
    void setAnimatable(std::shared_ptr<VROAnimatable> animatable) {
        _animatable = animatable;
    }
    
    /*
     Set a function to invoke when the animation completes.
     */
    void setFinishCallback(std::function<void(VROAnimatable *const)> callback) {
        _finishCallback = callback;
    }
    
    /*
     Move the property to its value corresponding to t [0, 1].
     */
    virtual void processAnimationFrame(float t) = 0;
    
    /*
     Immediately finish this animation by moving its value to the
     end state.
     */
    virtual void finish() = 0;
    
    /*
     Invoke after the animation is completed.
     */
    void onTermination() {
        finish();
        
        std::shared_ptr<VROAnimatable> animatable = _animatable.lock();
        if (animatable == nullptr){
            return;
        }

        animatable->onAnimationFinished();
        if (_finishCallback) {
            _finishCallback(animatable.get());
        }
    }
    
protected:
    
    std::weak_ptr<VROAnimatable> _animatable;
    std::function<void(VROAnimatable *const)> _finishCallback;
    
};

#endif /* VROAnimation_h */
