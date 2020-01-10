//
//  VROVector4f.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/30/15.
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

#ifndef VROVector4f_h
#define VROVector4f_h

#include <stdlib.h>
#include <math.h>
#include <string>

class VROVector4f {
public:
    float x;
    float y;
    float z;
    float w;
    
    VROVector4f();
    VROVector4f(const float *components, int count);
    VROVector4f(float x, float y, float z, float w);
    VROVector4f(const VROVector4f &vector);
    virtual ~VROVector4f();
    
    VROVector4f &operator*=(const float multiplier) {
        x *= multiplier;
        y *= multiplier;
        z *= multiplier;
        w *= multiplier;
        return *this;
    }
    
    VROVector4f &operator/=(const float divisor) {
        x /= divisor;
        y /= divisor;
        z /= divisor;
        w /= divisor;
        return *this;
    }
    
    VROVector4f &operator+=(const VROVector4f &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }
    
    /*
     Copy operations.
     */
    void set(const VROVector4f &value);
    void set(float x, float y, float z, float w);
    
    /*
     Basic arithmetic.
     */
    void add(const VROVector4f &vB, VROVector4f *result) const;
    void addScaled(const VROVector4f &scaledB, float scale, VROVector4f *result) const;
    void subtract(const VROVector4f &vB, VROVector4f *result) const;
    void scale(float factor, VROVector4f *result) const;
    
    /*
     Midpoints and distances.
     */
    void   midpoint(const VROVector4f &other, VROVector4f *result) const;
    float  magnitude() const;
    
    /*
     Basic vector operations.
     */
    float  dot(const VROVector4f &vB) const;
    VROVector4f normalize() const;
    VROVector4f interpolate(VROVector4f other, float t);
    
    /*
     Clearing.
     */
    void   clear();
    bool   isZero() const;
    
    /*
     Hashing.
     */
    bool isEqual(const VROVector4f &vector) const;
    int hash() const;
    
    /*
     Utilities.
     */
    std::string toString() const;
};

/*
 Operator overloads.
 */
inline VROVector4f operator+(VROVector4f lhs, const VROVector4f& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    lhs.w += rhs.w;
    return lhs; // return the result by value
}

inline VROVector4f operator*(VROVector4f lhs, const float multiplier) {
    lhs.x *= multiplier;
    lhs.y *= multiplier;
    lhs.z *= multiplier;
    lhs.w *= multiplier;
    return lhs; // return the result by value
}

inline VROVector4f operator/(VROVector4f lhs, const float divisor) {
    lhs.x /= divisor;
    lhs.y /= divisor;
    lhs.z /= divisor;
    lhs.w /= divisor;
    return lhs; // return the result by value
}

#endif /* VROVector4f_h */
