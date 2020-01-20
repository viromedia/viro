//
//  VROFieldOfView.cpp
//  ViroRenderer
//
//  Created by Raj Advani on 10/23/15.
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

#ifndef VROFieldOfView_h
#define VROFieldOfView_h

#include "VROMath.h"

enum class VROFieldOfViewAxis {
    X, Y
};

/*
 Represents the field of view in each direction, in degrees.
 */
class VROFieldOfView {
    
public:
    
    VROFieldOfView() :
        _left(s_defaultViewAngle),
        _right(s_defaultViewAngle),
        _bottom(s_defaultViewAngle),
        _top(s_defaultViewAngle)
    {}
    
    VROFieldOfView(float left, float right, float bottom, float top) :
        _left(left),
        _right(right),
        _bottom(bottom),
        _top(top)
    {}
    
    VROFieldOfView(const VROFieldOfView *other) :
        _left(other->_left),
        _right(other->_right),
        _bottom(other->_bottom),
        _top(other->_top)
    {}
    
    void setLeft(float left) {
        _left = left;
    }
    float getLeft() const {
        return _left;
    }
    
    void setRight(float right) {
        _right = right;
    }
    float getRight() const {
        return _right;
    }
    
    void setBottom(float bottom) {
        _bottom = bottom;
    }
    float getBottom() const {
        return _bottom;
    }
    
    void setTop(float top) {
        _top = top;
    }
    float getTop() const {
        return _top;
    }
    
    VROMatrix4f toPerspectiveProjection(float near, float far) {
        float left   = -tanf(degrees_to_radians(_left))   * near;
        float right  =  tanf(degrees_to_radians(_right))  * near;
        float bottom = -tanf(degrees_to_radians(_bottom)) * near;
        float top    =  tanf(degrees_to_radians(_top))    * near;
        
        return VROMathComputeFrustum(left, right, bottom, top, near, far);
    }
    
    bool equals(const VROFieldOfView *other) const {
        if (other == nullptr) {
            return false;
        }
        else if (other == this) {
            return true;
        }
        return (_left == other->_left) &&
               (_right == other->_right) &&
               (_bottom == other->_bottom) &&
               (_top == other->_top);
    }
    
private:
    
    constexpr static float s_defaultViewAngle = 40.0f;
    
    float _left;
    float _right;
    float _bottom;
    float _top;
    
};

#endif
