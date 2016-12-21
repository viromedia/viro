//
//  VRORenderContext.hpp
//  ViroRenderer
//
//  Created by Raj Advani on 10/13/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VRORenderContext_h
#define VRORenderContext_h

#include <stdio.h>
#include <vector>
#include <memory>
#include "VROFrameListener.h"
#include "VROMatrix4f.h"
#include "VROVector3f.h"
#include "VROQuaternion.h"
#include "VROCamera.h"

class VROFrameSynchronizer;
enum class VROEyeType;

/*
 Holds data specific to the current frame. Includes things like transformation
 matrices. There is nothing driver or device specific contained here.
 */
class VRORenderContext {
    
public:
    
    VRORenderContext(std::shared_ptr<VROFrameSynchronizer> synchronizer) :
        _frame(0),
        _frameSynchronizer(synchronizer) {
        
    }
    
    int getFrame() const {
        return _frame;
    }
    void setFrame(int frame) {
        _frame = frame;
    }
    
    void setEyeType(VROEyeType eye) {
        _eye = eye;
    }
    VROEyeType getEyeType() const {
        return _eye;
    }
    
    void setZNear(float zNear) {
        _zNear = zNear;
    }
    float getZNear() const {
        return _zNear;
    }
    
    void setZFar(float zFar) {
        _zFar = zFar;
    }
    float getZFar() const {
        return _zFar;
    }
    
    void setProjectionMatrix(VROMatrix4f projectionMatrix) {
        _projectionMatrix = projectionMatrix;
    }
    void setViewMatrix(VROMatrix4f viewMatrix) {
        _viewMatrix = viewMatrix;
    }
    void setEnclosureViewMatrix(VROMatrix4f enclosureViewMatrix) {
        _enclosureViewMatrix = enclosureViewMatrix;
    }
    void setHUDViewMatrix(VROMatrix4f hudViewMatrix) {
        _hudViewMatrix = hudViewMatrix;
    }
    void setCamera(VROCamera camera) {
        _camera = camera;
    }
    
    VROMatrix4f getProjectionMatrix() const {
        return _projectionMatrix;
    }
    VROMatrix4f getViewMatrix() const {
        return _viewMatrix;
    }
    VROMatrix4f getEnclosureViewMatrix() const {
        return _enclosureViewMatrix;
    }
    VROMatrix4f getHUDViewMatrix() const {
        return _hudViewMatrix;
    }
    
    const VROCamera &getCamera() const {
        return _camera;
    }
    
    std::shared_ptr<VROFrameSynchronizer> getFrameSynchronizer() const {
        return _frameSynchronizer;
    }
    
private:
    
    int _frame;
    VROEyeType _eye;
    
    /*
     The standard view and projection matrices. The view matrix is specific for
     the eye currently being rendered (it includes the stereo translation).
     */
    VROMatrix4f _projectionMatrix;
    VROMatrix4f _viewMatrix;
    
    /*
     The view matrix for camera enclosure objects (e.g. skyboxes).
     */
    VROMatrix4f _enclosureViewMatrix;
    
    /*
     The view matrix for objects rendered on the HUD. This is a stereo view matrix:
     it essentially removes head rotation and the camera, but retains the interlens
     distance translation.
     */
    VROMatrix4f _hudViewMatrix;
    
    /*
     The camera used for this frame.
     */
    VROCamera _camera;
    
    /*
     The near and far clipping planes.
     */
    float _zNear, _zFar;
    
    /*
     Synchronizer used to add or remove frame listeners.
     */
    std::shared_ptr<VROFrameSynchronizer> _frameSynchronizer;
    
};

#endif /* VRORenderContext_h */
