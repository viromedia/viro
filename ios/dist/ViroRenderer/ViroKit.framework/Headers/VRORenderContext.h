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
#include "VROFrameScheduler.h"

class VROFrameSynchronizer;
class VRORenderTarget;
class VROTexture;
class VROPencil;
class VROInputControllerBase;
enum class VROEyeType;

/*
 Holds data specific to the current frame. Includes things like transformation
 matrices. There is nothing driver or device specific contained here.
 
 Objects should *not* hold onto the VRORenderContext, as it is replaced each
 frame with fresh data.
 */
class VRORenderContext {
    
public:
    
    VRORenderContext(std::shared_ptr<VROFrameSynchronizer> synchronizer,
                     std::shared_ptr<VROFrameScheduler> scheduler) :
        _frame(0),
        _frameSynchronizer(synchronizer),
        _frameScheduler(scheduler) {
        
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
    void setOrthographicMatrix(VROMatrix4f orthographicMatrix) {
        _orthographicMatrix = orthographicMatrix;
    }
    void setCamera(VROCamera camera) {
        _camera = camera;
    }
    void setPreviousCamera(VROCamera camera) {
        _previousCamera = camera;
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
    VROMatrix4f getOrthographicMatrix() const {
        return _orthographicMatrix;
    }
    
    std::shared_ptr<VROTexture> getShadowMap() const {
        return _shadowMap;
    }
    void setShadowMap(std::shared_ptr<VROTexture> shadowMap) {
        _shadowMap = shadowMap;
    }
    
    std::shared_ptr<VROTexture> getIrradianceMap() const {
        return _irradianceMap;
    }
    void setIrradianceMap(std::shared_ptr<VROTexture> irradianceMap) {
        _irradianceMap = irradianceMap;
    }
    
    const VROCamera &getCamera() const {
        return _camera;
    }
    const VROCamera &getPreviousCamera() const {
        return _previousCamera;
    }
    
    void setRenderTarget(std::shared_ptr<VRORenderTarget> target) {
        _renderTarget = target;
    }
    const std::shared_ptr<VRORenderTarget> getRenderTarget() const {
        return _renderTarget;
    }
    
    std::shared_ptr<VROFrameSynchronizer> getFrameSynchronizer() const {
        return _frameSynchronizer;
    }
    std::shared_ptr<VROFrameScheduler> getFrameScheduler() const {
        return _frameScheduler;
    }
    
    void setFPS(double fps) {
        _fps = fps;
    }
    double getFPS() const {
        return _fps;
    }

    void setPencil(std::shared_ptr<VROPencil> pencil){
        _pencil = pencil;
    }
    std::shared_ptr<VROPencil> getPencil() const {
        return _pencil;
    }

    void setInputController(std::shared_ptr<VROInputControllerBase> inputController) {
        _inputController = inputController;
    }
    std::shared_ptr<VROInputControllerBase> getInputController() const {
        return _inputController;
    }

private:
    
    int _frame;
    VROEyeType _eye;
    double _fps;
    
    /*
     The target to which we are currently rendering.
     */
    std::shared_ptr<VRORenderTarget> _renderTarget;
    
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
     The projection matrix used for orthographically rendered geometries.
     */
    VROMatrix4f _orthographicMatrix;
    
    /*
     The camera used for this frame.
     */
    VROCamera _camera;
    
    /*
     The camera used during the previous frame. Useful for computing deltas, if
     required.
     */
    VROCamera _previousCamera;
    
    /*
     The near and far clipping planes.
     */
    float _zNear, _zFar;
    
    /*
     Synchronizer used to add or remove frame listeners.
     */
    std::shared_ptr<VROFrameSynchronizer> _frameSynchronizer;
    
    /*
     Scheduler used for queueing and executing rendering thread tasks.
     */
    std::shared_ptr<VROFrameScheduler> _frameScheduler;
    
    /*
     The texture containing shadow maps for all lights. This is a depth texture array.
     */
    std::shared_ptr<VROTexture> _shadowMap;
    
    /*
     Diffuse irradiance map used for PBR image-based lighting.
     */
    std::shared_ptr<VROTexture> _irradianceMap;

    /*
     VROPencil is used for drawing a list of VROPolylines in a separate render pass,
     after having rendered the scene, mainly for representing debug information.
     */
    std::shared_ptr<VROPencil> _pencil;

    /*
     The input controller being used.
     */
    std::shared_ptr<VROInputControllerBase> _inputController;

};

#endif /* VRORenderContext_h */
