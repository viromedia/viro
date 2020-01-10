//
//  VROMatrix4f.h
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

#ifndef VROMATRIX_H_
#define VROMATRIX_H_

#include <stdlib.h>
#include <math.h>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class VROVector3f;
class VROVector4f;
class VROQuaternion;

static float kMatrixEqualityEpsilon = 0.001;

class VROMatrix4f {
public:
    
    static VROMatrix4f identity();
    
    float &operator[] (const int index) {
        return _mtx[index];
    }
    float const &operator[](int index) const {
        return _mtx[index];
    }
    bool operator==(const VROMatrix4f& r) const {
        for (int i = 0; i < 16; i++) {
            if (fabs(_mtx[i] - r._mtx[i]) > kMatrixEqualityEpsilon) {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const VROMatrix4f& r) const {
        for (int i = 0; i < 16; i++) {
            if (fabs(_mtx[i] - r._mtx[i]) > kMatrixEqualityEpsilon) {
                return true;
            }
        }
        return false;
    }

    VROMatrix4f() noexcept;
    VROMatrix4f(const float *matrix);
    VROMatrix4f(const glm::mat4x4 mat);

    void toIdentity();
    bool isIdentity() const;
    void copy(const VROMatrix4f &copy);

    /*
     Scale.
     */
    void scale(float x, float y, float z);

    /*
     Rotation.
     */
    void rotateX(float angleRad);
    void rotateY(float angleRad);
    void rotateZ(float angleRad);
    void rotate(float angleRad, const VROVector3f &origin, const VROVector3f &dir);
    void rotate(const VROQuaternion &rotation);
    void setRotationCenter(const VROVector3f &center, const VROVector3f &translation);

    /*
     Translation.
     */
    void translate(float x, float y, float z);
    void translate(const VROVector3f &vector);

    /*
     Multiplication.
     */
    VROMatrix4f multiply(const VROMatrix4f &matrix) const;
    VROVector3f multiply(const VROVector3f &vector) const;
    VROVector4f multiply(const VROVector4f &vector) const;
    
    /*
     Decomposition into affine transforms. These methods only work on affine 
     matrices. To extract rotation, the scale factors are required.
     */
    VROVector3f   extractScale() const;
    VROQuaternion extractRotation(VROVector3f scale) const;
    VROVector3f   extractTranslation() const;
    
    /*
     Other operations.
     */
    VROMatrix4f transpose() const;
    VROMatrix4f invert() const;
    const float *getArray() const {
        return _mtx;
    }
    
    std::string toString() const;
    
private:
    
    /*
     The 16-float data for this matrix.
     */
    float _mtx[16];
    
};

inline VROMatrix4f operator*(const VROMatrix4f &lhs, const VROMatrix4f &rhs) {
    return lhs.multiply(rhs);
}

#endif /* VROMATRIX_H_ */
