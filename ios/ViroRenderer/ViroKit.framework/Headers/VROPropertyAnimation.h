//
//  VROPropertyAnimation.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/28/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROPropertyAnimation_h
#define VROPropertyAnimation_h

#include <stdio.h>
#include <string>
#include <memory>


/*
 Represents a single animated property of a node, material, or something else.
 */
class VROPropertyAnimation {
    
public:
    
    static std::shared_ptr<VROPropertyAnimation> parse(const std::string &name, const std::string &value);
    
    VROPropertyAnimation(std::string propertyName, float value, bool isAdditive) :
        _propertyName(propertyName),
        _value(value),
        _isAdditive(isAdditive) {}
    virtual ~VROPropertyAnimation() {}
    
    std::string getPropertyName() const { return _propertyName; }
    float getValue() const { return _value; }
    bool isAdditive() const { return _isAdditive; }
    
    std::string toString() const;
    
private:
    
    const std::string _propertyName;
    const float _value;
    const bool _isAdditive;
    
};

#endif /* VROPropertyAnimation_h */
