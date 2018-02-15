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
 Animated property values can be of the following types.
 */
enum class VROValueType {
    Float,
    Int
};

/*
 Encapsulates different types of animation values.
 */
class VROAnimationValue {
public:
    VROValueType type;
    float valueFloat;
    int valueInt;
};

/*
 Determines if animation assigns values, or adds/multiplies.
 */
enum class VROAnimationOperation {
    Add,
    Multiply,
    Assign
};

/*
 Represents a single animated property of a node (e.g. position, scale, rotation).
 */
class VROPropertyAnimation {
    
public:
    
    static std::shared_ptr<VROPropertyAnimation> parse(const std::string &name, const std::string &value);
    
    VROPropertyAnimation(std::string propertyName, VROAnimationValue value, VROAnimationOperation op) :
        _propertyName(propertyName),
        _value(value),
        _op(op) {}
    virtual ~VROPropertyAnimation() {}
    
    std::string getPropertyName() const { return _propertyName; }
    VROAnimationValue getValue() const { return _value; }
    VROAnimationOperation getOp() const { return _op; }
    
    float processOp(float input) const {
        switch (_op) {
            case VROAnimationOperation::Assign:
                return _value.valueFloat;
            case VROAnimationOperation::Add:
                return _value.valueFloat + input;
            case VROAnimationOperation::Multiply:
                return _value.valueFloat * input;
                
            default:
                return _value.valueFloat;
        }
    }
    
    std::string toString() const;
    
private:
    
    const std::string _propertyName;
    const VROAnimationValue _value;
    const VROAnimationOperation _op;
    
};

#endif /* VROPropertyAnimation_h */
