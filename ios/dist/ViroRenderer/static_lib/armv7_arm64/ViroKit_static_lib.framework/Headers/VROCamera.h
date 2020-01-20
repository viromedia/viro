//
//  VROCamera.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/13/15.
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

#ifndef VROCamera_h
#define VROCamera_h

#include "VROVector3f.h"
#include "VROQuaternion.h"
#include "VROMatrix4f.h"
#include "VROViewport.h"
#include "VROFrustum.h"
#include "VROFieldOfView.h"

enum class VROCameraRotationType {
    Standard,
    Orbit
};

/*
 The default forward and up vectors. These are rotated by the base and head
 rotation to derive the actual forward and up vectors.
 */
static const VROVector3f kBaseForward = { 0, 0, -1 };
static const VROVector3f kBaseUp = { 0, 1, 0 };

class VROCamera {
    
public:
    
    VROCamera();
    virtual ~VROCamera();
    
    void setPosition(VROVector3f position);
    void setHeadRotation(VROMatrix4f headRotation);
    void setBaseRotation(VROMatrix4f baseRotation);
    void setViewport(VROViewport viewport);
    void setFOV(VROFieldOfView fov);
    void setProjection(VROMatrix4f projection);
    
    VROVector3f getPosition() const {
        return _position;
    }
    VROVector3f getForward() const {
        return _forward;
    }
    VROVector3f getUp() const {
        return _up;
    }
    VROQuaternion getRotation() const {
        return _rotation;
    }
    VROMatrix4f getLookAtMatrix() const {
        return _lookAtMatrix;
    }
    VROMatrix4f getProjection() const {
        return _projection;
    }
    const VROFrustum &getFrustum() const {
        return _frustum;
    }
    VROFieldOfView getFieldOfView() const {
        return _fov;
    }
    VROViewport getViewport() const {
        return _viewport;
    }
    float getNCP() const {
        return _ncp;
    }
    float getFCP() const {
        return _fcp;
    }
    void computeLookAtMatrix();
    void computeFrustum();
    
    float getWorldPerScreen(float distance) const;
private:
    
    VROVector3f _position;
    
    /*
     The camera forward vector. Normalized vector indicating where the camera
     is pointing.
     */
    VROVector3f _forward;
    
    /*
     The camera up vector. Normalized vector indicating which direction is 'up'.
     */
    VROVector3f _up;
    
    /*
     The camera quaternion represents the rotation from (0, 0, -1) required to
     achieve the camera's current orientation. This factors in both base 
     rotation and head rotation.
     */
    VROQuaternion _rotation;
    
    /*
     The current head rotation. Head rotation is set by the user's HMD orientation.
     Total rotation is head rotation plus base rotation.
     */
    VROMatrix4f _headRotation;
    
    /*
     The base rotation. This is set by the application. Total rotation is head
     rotation plus base rotation.
     */
    VROMatrix4f _baseRotation;

    /*
     The last computed lookAt matrix for this camera. This matrix combines all rotation
     and position information into one unified view matrix. Note like all matrices in
     the VROCamera, this is eye-independent (the eyeFromHeadMatrix is not considered).
     */
    VROMatrix4f _lookAtMatrix;
    
    /*
     The projection matrix associated with this camera.
     */
    VROMatrix4f _projection;
    
    /*
     The frustum associated with this camera. Derived from the lookAtMatrix and the
     projection matrix.
     */
    VROFrustum _frustum;
    
    /*
     The current viewport and FOV.
     */
    VROViewport _viewport;
    VROFieldOfView _fov;

    /*
     The camera's near and far clipping planes.
     */
    float _ncp, _fcp;
    
    void onRotationChanged();
    
};

class VROCameraDelegate {
public:
    VROCameraDelegate() {};
    virtual ~VROCameraDelegate() {};
    virtual void onCameraTransformationUpdate(VROVector3f pos,
                                              VROVector3f rot,
                                              VROVector3f forward) = 0;
};

#endif /* VROCamera_h */
