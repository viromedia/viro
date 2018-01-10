//
//  VRONodeCamera.h
//  ViroRenderer
//
//  Created by Raj Advani on 3/24/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VRONodeCamera_h
#define VRONodeCamera_h

#include "VROAnimatable.h"
#include "VROVector3f.h"
#include "VROQuaternion.h"

enum class VROCameraRotationType;

/*
 Node cameras are attached to nodes, and derive their base position and rotation
 from said parent node. The "active" node camera each frame becomes the 
 point of view from which the scene is rendered in that frame. To designate
 a node camera as active, set the VROView's pointOfView property to that
 camera's node.
 */
class VRONodeCamera : public VROAnimatable {
    
public:
    
    VRONodeCamera();
    virtual ~VRONodeCamera();
    
    void setPosition(VROVector3f position);
    void setBaseRotation(VROQuaternion baseRotation);
    void setRotationType(VROCameraRotationType type);
    void setOrbitFocalPoint(VROVector3f focalPt);
    
    VROVector3f getPosition() const {
        return _position;
    }
    VROQuaternion getBaseRotation() const {
        return _baseRotation;
    }
    VROCameraRotationType getRotationType() const {
        return _rotationType;
    }
    VROVector3f getOrbitFocalPoint() const {
        return _orbitFocalPt;
    }
    
private:
    
    /*
     The position of the camera, relative to the node's coordinate system.
     */
    VROVector3f _position;
    
    /*
     The base rotation. This is set by the application. Total rotation is head
     rotation plus base rotation plus the rotation of the parent node.
     */
    VROQuaternion _baseRotation;
    
    /*
     The camera rotation type (orbit around a focal point, or standard rotation).
     */
    VROCameraRotationType _rotationType;
    
    /*
     If in orbit mode, this is the point that the camera focuses on, from its current
     position. Specified in the coordinate system of the parent node.
     */
    VROVector3f _orbitFocalPt;
    
};

#endif /* VRONodeCamera_hpp */
