//
//  VROAnimationChain.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/28/16.
//  Copyright © 2016 Viro Media. All rights reserved.
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

#ifndef VROAnimationChain_h
#define VROAnimationChain_h

#include <vector>
#include "VROExecutableAnimation.h"

enum class VROAnimationChainExecution {
    Serial,
    Parallel
};

/*
 An animation chain is a list of animation groups that can execute serially
 or in parallel. This follows the composite pattern.
 */
class VROAnimationChain : public VROExecutableAnimation, public std::enable_shared_from_this<VROAnimationChain> {
    
public:
    
    VROAnimationChain(std::vector<std::shared_ptr<VROExecutableAnimation>> &animations,
                      VROAnimationChainExecution execution) :
        _animations(animations),
        _execution(execution),
        _numComplete(0) {}
    virtual ~VROAnimationChain() {}
    
    std::shared_ptr<VROExecutableAnimation> copy();
    
    void execute(std::shared_ptr<VRONode> node,
                 std::function<void()> onFinished);

    // This function adds an animation to the end of the chain.
    void addAnimation(std::shared_ptr<VROExecutableAnimation> animation);
    void pause();
    void resume();
    void terminate(bool jumpToEnd);
    void preload();
    
    void setDuration(float durationSeconds);
    float getDuration() const;

    void setTimeOffset(float timeOffset);
    float getTimeOffset() const;

    void setSpeed(float speed);
    
    const std::vector<std::shared_ptr<VROExecutableAnimation>> &getAnimations() const {
        return _animations;
    }

    std::string toString() const;

private:
    
    void executeSerial(std::shared_ptr<VRONode> node, int groupIndex,
                       std::function<void()> onFinished);
    void executeParallel(std::shared_ptr<VRONode> node,
                         std::function<void()> onFinished);
    void animateGroup(int groupIndex);
    
    std::vector<std::shared_ptr<VROExecutableAnimation>> _animations;
    VROAnimationChainExecution _execution;
    int _numComplete;
    
};

#endif /* VROAnimationChain_h */
