//
//  VROAnimationGroup.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/28/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROAnimationGroup_h
#define VROAnimationGroup_h

#include <stdio.h>
#include <vector>
#include <map>
#include "VROPropertyAnimation.h"
#include "VROMaterialAnimation.h"
#include "VROTimingFunction.h"
#include "VROExecutableAnimation.h"

class VRONode;
class VROLazyMaterial;

/*
 An animation group is a set of property and material animations that
 execute simultaneously.
 */
class VROAnimationGroup : public VROExecutableAnimation, public std::enable_shared_from_this<VROAnimationGroup> {
    
public:
    
    static std::shared_ptr<VROAnimationGroup> parse(float duration, float delay, std::string functionName,
                                                    std::map<std::string, std::string> &properties,
                                                    std::vector<std::shared_ptr<VROLazyMaterial>> materials);
    
    VROAnimationGroup(float durationSeconds, float delaySeconds,
                      VROTimingFunctionType timingFunction,
                      std::map<std::string, std::shared_ptr<VROPropertyAnimation>> &propertyAnimations,
                      std::vector<std::shared_ptr<VROMaterialAnimation>> &materialAnimations) :
        _duration(durationSeconds),
        _delay(delaySeconds),
        _timingFunctionType(timingFunction),
        _propertyAnimations(propertyAnimations),
        _materialAnimations(materialAnimations) {}
    virtual ~VROAnimationGroup() {}
    
    std::shared_ptr<VROExecutableAnimation> copy();
    
    void execute(std::shared_ptr<VRONode> node,
                 std::function<void()> onFinished);
    
    void pause();
    void resume();
    void terminate(bool jumpToEnd);

    void setDuration(float durationSeconds);
    float getDuration() const;

    std::string toString() const;
    
private:
    
    static VROTimingFunctionType parseTimingFunction(std::string &name);
    
    float _duration;
    float _delay;
    VROTimingFunctionType _timingFunctionType;
    
    std::shared_ptr<VROTransaction> _transaction;
    
    void animatePosition(std::shared_ptr<VRONode> &node);
    void animateScale(std::shared_ptr<VRONode> &node);
    void animateColor(std::shared_ptr<VRONode> &node);
    void animateOpacity(std::shared_ptr<VRONode> &node);
    void animateRotation(std::shared_ptr<VRONode> &node);
    void animateMaterial(std::shared_ptr<VRONode> &node);
    
    std::map<std::string, std::shared_ptr<VROPropertyAnimation>> _propertyAnimations;
    std::vector<std::shared_ptr<VROMaterialAnimation>> _materialAnimations;
  
};

#endif /* VROAnimationGroup_h */
