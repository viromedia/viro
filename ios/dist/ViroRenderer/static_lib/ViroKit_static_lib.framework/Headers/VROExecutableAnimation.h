//
//  VROExecutableAnimation.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/4/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROExecutableAnimation_h
#define VROExecutableAnimation_h

#include <memory>
#include <functional>

class VRONode;
class VROTransaction;

class VROExecutableAnimation {

public:
    VROExecutableAnimation() {
       _timeOffset = 0;
       _speed = 1.0f;
    }

    virtual ~VROExecutableAnimation() {}
    
    /*
     Produce a copy of this animation.
     */
    virtual std::shared_ptr<VROExecutableAnimation> copy() = 0;
    
    /*
     Preload the animation, performing any necessary setup work now instead
     of when the animation runs.
     */
    virtual void preload() = 0;
    
    /*
     Execute this animation. The onFinished() callback will be invoked when the
     animation is fully executed (if it has children, this is when the last child
     finishes executing).
     */
    virtual void execute(std::shared_ptr<VRONode> node,
                         std::function<void()> onFinished) = 0;

    /*
     Override the set duration for this animation, in seconds. This should typically
     only be done on an animation copy to prevent modifying the original animation's
     standard duration.
     */
    virtual void setDuration(float durationSeconds) = 0;

    virtual void setTimeOffset(float timeOffset) {
        _timeOffset = timeOffset;
    }

    float getTimeOffset() const {
        return _timeOffset;
    }

    virtual void setSpeed(float speed) {
        _speed = speed;
    }

    float getSpeed() const {
        return _speed;
    }

    /*
     Returns the duration of this animation, in seconds. If this animation contains
     multiple sub-animations, this will return the longest animation.
     */
    virtual float getDuration() const = 0;
    
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void terminate(bool jumpToEnd) = 0;
    
    virtual std::string toString() const = 0;

protected:
    float _timeOffset;
    float _speed;
};

#endif /* VROExecutableAnimation_h */
