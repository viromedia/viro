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
    VROExecutableAnimation() {}
    virtual ~VROExecutableAnimation() {}
    
    /*
     Produce a copy of this animation.
     */
    virtual std::shared_ptr<VROExecutableAnimation> copy() = 0;
    
    /*
     Execute this animation. The onFinished() callback will be invoked when the
     animation is fully executed (if it has children, this is when the last child
     finishes executing).
     */
    virtual void execute(std::shared_ptr<VRONode> node,
                         std::function<void()> onFinished) = 0;
    
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void terminate() = 0;
    
    virtual std::string toString() const = 0;
};

#endif /* VROExecutableAnimation_h */
