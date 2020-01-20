//
//  VROMath.h
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

#ifndef VROMath_h
#define VROMath_h

#include <stdio.h>
#include <vector>
#include <cstdlib>
#include "VROVector3f.h"
#include "VROVector4f.h"
#include "VROMatrix4f.h"
#include "VROQuaternion.h"
#include "VROBoundingBox.h"
#include "VRODefines.h"

static float kRoundingErrorFloat = 0.00001;
static float kEpsilon = 0.00000001;

VROMatrix4f matrix_from_scale(float sx, float sy, float sz);
VROMatrix4f matrix_from_translation(float x, float y, float z);
VROMatrix4f matrix_from_perspective_fov_aspectLH(const float fovY, const float aspect,
                                                     const float nearZ, const float farZ);

/*
 Compute a look-at matrix for a right handed coordinate system.
 */
VROMatrix4f VROMathComputeLookAtMatrix(VROVector3f eye, VROVector3f forward, VROVector3f up);

/*
 Compute a frustum or perspective projection for a right handed coordinate system.
 */
VROMatrix4f VROMathComputeFrustum(float left, float right, float bottom, float top,
                                  float znear, float zfar);
VROMatrix4f VROMathComputePerspectiveProjection(float fovyInDegrees, float aspectRatio,
                                                float znear, float zfar);

/*
 Compute an orthographic projection for a right handed coordinate system.
 */
VROMatrix4f VROMathComputeOrthographicProjection(float left, float right, float bottom, float top,
                                                 float near, float far);

double degrees_to_radians(double degrees);
double radians_to_degrees(double radians);

float clamp(float val, float min, float max);
float random(float min, float max);
VROVector3f random(VROVector3f min, VROVector3f max);

/*
 4x4 column-major matrix operations.
 */
void VROMathMultVectorByMatrix(const float *matrix, const float *input, float *output);
void VROMathMultVectorByMatrix_d(const double *matrix, const double *input, double *output);
void VROMathMultVectorByMatrix_fd(const float *matrix, const double *input, double *output);

void VROMathMultMatrices(const float *a, const float *b, float *r);
void VROMathMultMatrices_d(const double *a, const double *b, double *r);
void VROMathMultMatrices_dff(const double *a, const float *b, float *r);
void VROMathMultMatrices_ddf(const double *a, const double *b, float *r);
void VROMathMultMatrices_fdf(const float *a, const double *b, float *r);
void VROMathMultMatrices_dfd(const double *a, const float *b, double *r);
void VROMathMultMatrices_fdd(const float *a, const double *b, double *r);
void VROMathMultMatrices_ffd(const float *a, const float *b, double *r);

void VROMathMakeIdentity(float *m);
void VROMathMakeIdentity_d(double *m);

void VROMathTransposeMatrix(const float *src, float *transpose);

bool VROMathInvertMatrix(const float *src, float *inverse);
bool VROMathInvertMatrix_d(const double *src, double *inverse);

/*
 4x4 special matrix ops.
 */
void VROMathMultMatricesOptScale(const float *m1, const float *m0, float *d);
void VROMathMultMVP(const float *m1, const float *m0, float *d);
void VROMathMultVX(const float *vx, const float *m0, float *d);

/*
 Interpolation functions.
 */
float  VROMathInterpolate(float input, float inMin, float inMax, float outMin, float outMax);
double VROMathInterpolate_d(double input, double inMin, double inMax, double outMin, double outMax);
float  VROMathInterpolateKeyFrame(float input, const std::vector<float> &inputs, const std::vector<float> &outputs);
float  VROMathInterpolateKeyFrameIndex(float input, const std::vector<float> &inputs);
VROVector3f   VROMathInterpolateKeyFrameVector3f(float input, const std::vector<float> &inputs, const std::vector<VROVector3f> &outputs);
VROQuaternion VROMathInterpolateKeyFrameQuaternion(float input, const std::vector<float> &inputs, const std::vector<VROQuaternion> &outputs);
VROMatrix4f   VROMathInterpolateKeyFrameMatrix4f(float input, const std::vector<float> &inputs, const std::vector<VROMatrix4f> &outputs);
void   VROMathInterpolatePoint(const float *bottom, const float *top, float amount, int size, float *result);

/*
 * Clamps input between min and max
 */
double VROMathClamp(double input, double min, double max);

/*
 Array math
 */
float VROMathMin(const float values[], int count);
float VROMathMax(const float values[], int count);

/*
 Angle conversion.
 */
float toRadians(float degrees);
float toDegrees(float radians);

/*
 Rotation.
 */
void VROMathRotateAroundX(const VROVector3f &vector, float radians, VROVector3f *result);
void VROMathRotateAroundZ(const VROVector3f &vector, float radians, VROVector3f *result);

/*
 Normalize the given angle between [0,2PI] or [-PI,PI], and find the distance between two angles.
 */
float VROMathNormalizeAngle2PI(float rad);
float VROMathNormalizeAnglePI(float rad);
float VROMathAngleDistance(float radA, float radB);

/*
 Convert the given color from sRGB to linear space.
 */
VROVector3f VROMathConvertSRGBToLinearColor(VROVector3f sRGB);
VROVector4f VROMathConvertSRGBToLinearColor(VROVector4f sRGB);

/*
 Normalize the angles in the given vector between [0, 2PI].
 */
VROVector3f VROMathNormalizeAngles2PI(VROVector3f vector);

/*
 Take the fast (inverse) square root of the given number.
 */
float VROMathFastSquareRoot(float x);

/*
 Fast sin/cos methods. An input angle between [-PI, PI] will skip a
 range reduction step, and may perform slightly faster, but is
 unnessisary.
 */
void  VROMathFastSinCos(float x, float r[2]);

void  VROMathFastSinCos2x(const float *angles, float * r);

/**
 Determine whether point (x,y) is within polygon (x1,y1 to x2,y2 to x3,y3 to x4,y4 to x1,y1)
 Point on edge is considered within.
 Only for use with convex polygons.
 */
bool VROMathPointIsInPolygon(float x, float y, float x1, float y1,
                             float x2, float y2, float x3, float y3,
                             float x4, float y4);

/*
 Get the point on segment AB that is closest to p.
 */
VROVector3f VROMathGetClosestPointOnSegment(const VROVector3f A, const VROVector3f B, const VROVector3f p);

/* return the power of 2 that is equal or greater to the given value */
static inline uint32_t
power2_ceil(const uint32_t v) {
    return  (v < 2) ? v + 1 : 1 << (sizeof(uint32_t) * 8 - __builtin_clz(v - 1));
}

float VROMathReciprocal(float value);
float VROMathReciprocalSquareRoot(float value);

bool VROMathIsZero(const float a, const float tolerance = kRoundingErrorFloat);
bool VROMathEquals(const float a, const float b, const float tolerance = kRoundingErrorFloat);

float VROFloat16ToFloat(short fltInt16);
short VROFloatToFloat16(float value);

VROVector3f VROMathGetCenter(std::vector<VROVector3f> &vertices);
VROBoundingBox VROMathGetBoundingBox(std::vector<VROVector3f> &vertices);

uint32_t VROMathRoundUpToNextPow2(uint32_t v);

#endif /* VROMath_h */
