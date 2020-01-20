//
//  VROQuaternion.h
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

#ifndef __VROQUATERNION_H_INCLUDED__
#define __VROQUATERNION_H_INCLUDED__

#include "VROVector3f.h"
#include "VROMatrix4f.h"

static float kRoundingErrorQuatFloat = 0.00001;

//! Quaternion class for representing rotations.
/** It provides cheap combinations and avoids gimbal locks.
Also useful for interpolations. */
class VROQuaternion {
	public:

		//! Default Constructor
		VROQuaternion() noexcept : X(0.0f), Y(0.0f), Z(0.0f), W(1.0f) {}

		//! Constructor
		VROQuaternion(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }

		//! Constructor which converts euler angles (radians) to a quaternion
		VROQuaternion(float x, float y, float z);

		//! Constructor which converts euler angles (radians) to a quaternion
		VROQuaternion(const VROVector3f& vec);

		//! Constructor which converts a matrix to a quaternion
		VROQuaternion(const VROMatrix4f& mat);

		//! Equalilty operator
		bool operator==(const VROQuaternion &other) const;

		//! inequality operator
		bool operator!=(const VROQuaternion &other) const;
    
		//! Matrix assignment operator
		VROQuaternion& operator=(const VROMatrix4f& other);

		//! Add operator
		VROQuaternion operator+(const VROQuaternion &other) const;
    
        //! Subtract operator
        VROQuaternion operator-(const VROQuaternion &other) const;

		//! Multiplication operator
		//! Be careful, unfortunately the operator order here is opposite of that in CVROMatrix4f::operator*
		VROQuaternion operator*(const VROQuaternion &other) const;

		//! Multiplication operator with scalar
		VROQuaternion operator*(float s) const;

		//! Multiplication operator with scalar
		VROQuaternion &operator*=(float s);

		//! Multiplication operator
		VROVector3f operator*(const VROVector3f& v) const;

		//! Multiplication operator
		VROQuaternion &operator*=(const VROQuaternion &other);

		//! Calculates the dot product
		float dotProduct(const VROQuaternion &other) const;
    
        // Get the magnitude of the angle represented by this quaternion.
        float getAngle() const;

		//! Sets new quaternion
		VROQuaternion &set(float x, float y, float z, float w);

		//! Sets new quaternion based on euler angles (radians)
		VROQuaternion &set(float x, float y, float z);

		//! Sets new quaternion based on euler angles (radians)
		VROQuaternion &set(const VROVector3f& vec);

		//! Sets new quaternion from other quaternion
		VROQuaternion &set(const VROQuaternion &quat);

		//! returns if this quaternion equals the other one, taking floating point rounding errors into account
		bool equals(const VROQuaternion &other,
                    const float tolerance = kRoundingErrorQuatFloat ) const;

		//! Normalizes the quaternion
		VROQuaternion &normalize();
    
        // Get the norm of the quaternion
        float getNorm();

		//! Creates a matrix from this quaternion
		VROMatrix4f getMatrix() const;

		//! Creates a matrix from this quaternion
		void getMatrix( VROMatrix4f &dest, const VROVector3f &translation=VROVector3f() ) const;

		/*!
			Creates a matrix from this quaternion
			Rotate about a center point
			shortcut for
			quaternion q;
			q.rotationFromTo ( vin[i].Normal, forward );
			q.getMatrixCenter ( lookat, center, newPos );

			VROMatrix4f m2;
			m2.setInverseTranslation ( center );
			lookat *= m2;

			VROMatrix4f m3;
			m2.setTranslation ( newPos );
			lookat *= m3;

		*/
		void getMatrixCenter( VROMatrix4f &dest, const VROVector3f &center, const VROVector3f &translation ) const;

		//! Creates a matrix from this quaternion
		void getMatrix_transposed( VROMatrix4f &dest ) const;

		//! Inverts this quaternion
		VROQuaternion &makeInverse();

		//! Set this quaternion to the linear interpolation between two quaternions
		/** \param q1 First quaternion to be interpolated.
		\param q2 Second quaternion to be interpolated.
		\param time Progress of interpolation. For time=0 the result is
		q1, for time=1 the result is q2. Otherwise interpolation
		between q1 and q2.
		*/
		static VROQuaternion lerp(VROQuaternion q1, VROQuaternion q2, float time);

		//! Set this quaternion to the result of the spherical interpolation between two quaternions
		/** \param q1 First quaternion to be interpolated.
		\param q2 Second quaternion to be interpolated.
		\param time Progress of interpolation. For time=0 the result is
		q1, for time=1 the result is q2. Otherwise interpolation
		between q1 and q2.
		\param threshold To avoid inaccuracies at the end (time=1) the
		interpolation switches to linear interpolation at some point.
		This value defines how much of the remaining interpolation will
		be calculated with lerp. Everything from 1-threshold up will be
		linear interpolation.
		*/
		static VROQuaternion slerp(VROQuaternion q1, VROQuaternion q2,
                                    float time, float threshold=.05f);

		//! Create quaternion from rotation angle and rotation axis.
		/** Axis must be unit length.
		The quaternion representing the rotation is
		q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k).
		\param angle Rotation Angle in radians.
		\param axis Rotation axis. */
		static VROQuaternion fromAngleAxis (float angle, const VROVector3f& axis);

		//! Fills an angle (radians) around an axis (unit vector)
		void toAngleAxis (float &angle, VROVector3f& axis) const;

		//! Output this quaternion to an euler angle (radians)
		VROVector3f toEuler() const;

		//! Set quaternion to identity
		VROQuaternion &makeIdentity();

		//! Set quaternion to represent a rotation from one vector to another.
		static VROQuaternion rotationFromTo(const VROVector3f& from, const VROVector3f& to);

		//! Quaternion elements.
		float X; // vectorial (imaginary) part
		float Y;
		float Z;
		float W; // real part
};

#endif

