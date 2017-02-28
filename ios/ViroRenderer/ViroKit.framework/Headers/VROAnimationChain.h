//
//  VROAnimationChain.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/28/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROAnimationChain_h
#define VROAnimationChain_h

#include <vector>
#include "VROExecutableAnimation.h"

enum class VROAnimationChainExecution {
    Serial,
    Parallel
};

/*
 An animation chain is a list of animation groups that execute
 serially. This follows the composite pattern.
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
    void terminate();
    
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
