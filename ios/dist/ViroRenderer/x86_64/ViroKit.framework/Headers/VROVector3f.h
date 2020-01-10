//
//  VROVector3h.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/15/15.
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

#ifndef VROVECTOR3F_H_
#define VROVECTOR3F_H_

#include <stdlib.h>
#include <math.h>
#include <string>

class VROVector3f {
public:
    float x;
    float y;
    float z;

    VROVector3f() noexcept;
    VROVector3f(float x, float y);
    VROVector3f(float x, float y, float z);
    
    VROVector3f &operator*=(const float multiplier) {
        x *= multiplier;
        y *= multiplier;
        z *= multiplier;
        return *this;
    }
    
    VROVector3f &operator/=(const float divisor) {
        x /= divisor;
        y /= divisor;
        z /= divisor;
        return *this;
    }
    
    VROVector3f &operator+=(const VROVector3f &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
    
    VROVector3f &operator-=(const VROVector3f &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }
    
    /*
     Find the absolute angle between this vector and the given vector. 
     */
    float angleWithVector(const VROVector3f &line) const;

    /*
     Find the angle between this vector and another vector. Both must be
     normalized. Return value in the range [0, PI].
     */
    float angleWithNormedVector(const VROVector3f &vector) const;

    /*
     Rotate the vector.
     */
    VROVector3f rotateZ(float angleRad) const;
    VROVector3f rotateAboutAxis(const VROVector3f &axisDir, const VROVector3f &axisPos, float angleRad) const;

    /*
     Intersect the line or ray defined by this vector and the given origin with the plane defined by the given point and normal.
     Store the intersection in the given point. Note that this is only valid if this vector has already been normalized.
     */
    bool lineIntersectPlane(const VROVector3f &point, const VROVector3f &normal,
                            const VROVector3f &origin, VROVector3f *intPt) const;
    bool rayIntersectPlane(const VROVector3f &point, const VROVector3f &normal,
                           const VROVector3f &origin, VROVector3f *intPt) const;

    /*
     Projects this point (in world coordinates) onto the given plane defined by the given point & normal
     */
    bool projectOnPlane(const VROVector3f &point, const VROVector3f &normal, VROVector3f *projPoint);

    /*
     Copy operations.
     */
    void set(const VROVector3f &value);
    void set(float x, float y, float z);

    /*
     Basic arithmetic.
     */
    VROVector3f add(VROVector3f vB) const;
    VROVector3f addScaled(const VROVector3f &scaledB, float scale) const;
    VROVector3f subtract(VROVector3f vB) const;
    VROVector3f scale(float factor) const;

    /*
     Midpoints and distances.
     */
    VROVector3f midpoint(const VROVector3f &other) const;
    float  distance(const VROVector3f &vB) const;
    float  distanceAccurate(const VROVector3f &vB) const;
    float  distanceXY(const VROVector3f &vB) const;
    float  distanceSquared(const VROVector3f &vB) const;
    float  magnitude() const;
    float  magnitudeXY() const;

    /*
     Basic vector operations.
     */
    float  dot(const VROVector3f &vB) const;
    VROVector3f cross(const VROVector3f &vB) const;
    VROVector3f normalize() const;
    VROVector3f interpolate(VROVector3f other, float t) const;

    /*
     Clearing.
     */
    void   clear();
    bool   isZero() const;

    /*
     Hashing.
     */
    bool isEqual(const VROVector3f &vector) const;
    int hash() const;

    /*
     Utilities.
     */
    std::string toString() const;
    void toArray(float *array) const;
    
    /*
     Operator overloads.
     */
    inline bool operator ==(const VROVector3f &other) const {
        return other.x == x && other.y == y && other.z == z;
    }
    
    inline bool operator !=(const VROVector3f &other) const {
        return !this->operator ==(other);
    }
};

/*
 Operator overloads.
 */
inline VROVector3f operator+(VROVector3f lhs, const VROVector3f& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs; // return the result by value
}

inline VROVector3f operator-(VROVector3f lhs, const VROVector3f& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs; // return the result by value
}

inline VROVector3f operator*(VROVector3f lhs, const float multiplier) {
    lhs.x *= multiplier;
    lhs.y *= multiplier;
    lhs.z *= multiplier;
    return lhs; // return the result by value
}

inline VROVector3f operator*(float multiplier, VROVector3f rhs) {
    rhs.x *= multiplier;
    rhs.y *= multiplier;
    rhs.z *= multiplier;
    return rhs; // return the result by value
}

inline VROVector3f operator*(VROVector3f lhs, VROVector3f rhs) {
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    lhs.z *= rhs.z;
    return lhs; // return the result by value
}

inline VROVector3f operator/(VROVector3f lhs, const float divisor) {
    lhs.x /= divisor;
    lhs.y /= divisor;
    lhs.z /= divisor;
    return lhs; // return the result by value
}

#endif /* VROVECTOR3F_H_ */
