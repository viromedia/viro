//
//  VROCamera.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/13/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROCamera_h
#define VROCamera_h

#include "VROVector3f.h"
#include "VROQuaternion.h"
#include "VROMatrix4f.h"
#include "VROViewport.h"
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
    VROMatrix4f computeLookAtMatrix() const;
    
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
     The current viewport and FOV.
     */
    VROViewport _viewport;
    VROFieldOfView _fov;
    
    void onRotationChanged();
    
};

#endif /* VROCamera_h */
