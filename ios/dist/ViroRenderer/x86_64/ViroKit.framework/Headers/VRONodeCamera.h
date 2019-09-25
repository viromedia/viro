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

class VRONode;
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
    void setFieldOfViewY(float fovy);
    void setRefNodeToCopyRotation(std::shared_ptr<VRONode> node);
    
    /*
     Return the ref node.
     */
    std::shared_ptr<VRONode> getRefNodeToCopyRotation() const {
        return _refNodeToCopyCameraRotation.lock();
    }
    
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
    float getFieldOfView() const {
        return _fov;
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

    /*
     The field of view for this camera, along the major (larger) axis. The minor axis
     FOV will be automatically computed from this and the viewport. If this is zero, then
     Viro will use the default FOV for the view. This value is ignored on VR and AR
     platforms, where the FOV is fixed by the eye settings or the camera. This value
     is given in degrees.

     Note that the major axis is the axis with the larger dimension: the X axis in landscape
     mode, or the Y axis in portrait mode. By specifying the FOV in terms of the major axis, Viro
     can keep the FOV consistent even upon orientation changes when the major/minor axes
     swap.
     */
    float _fov;
    
    /*
     Can be null. Reference node that is used to store the head rotation of the camera in a VRONode.
     */
    std::weak_ptr<VRONode> _refNodeToCopyCameraRotation;
    
};

#endif /* VRONodeCamera_hpp */
