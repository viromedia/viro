//
//  VRORenderContext.hpp
//  ViroRenderer
//
//  Created by Raj Advani on 10/13/15.
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
    
    VRORenderContext(std::shared_ptr<VROFrameSynchronizer> synchronizer) :
        _frame(0),
        _frameSynchronizer(synchronizer),
        _hdrEnabled(true),
        _pbrEnabled(true) {
        
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

    std::shared_ptr<VROTexture> getPrefilteredMap() const {
        return _prefilteredMap;
    }
    void setPrefilteredMap(std::shared_ptr<VROTexture> map) {
        _prefilteredMap = map;
    }

    std::shared_ptr<VROTexture> getBRDFMap() const {
        return _brdfMap;
    }
    void setBRDFMap(std::shared_ptr<VROTexture> map) {
        _brdfMap = map;
    }
    
    const VROCamera &getCamera() const {
        return _camera;
    }
    const VROCamera &getPreviousCamera() const {
        return _previousCamera;
    }
    
    std::shared_ptr<VROFrameSynchronizer> getFrameSynchronizer() const {
        return _frameSynchronizer;
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
    
    void setHDREnabled(bool enabled) {
        _hdrEnabled = enabled;
    }
    bool isHDREnabled() const {
        return _hdrEnabled;
    }
    
    void setPBREnabled(bool enabled) {
        _pbrEnabled = enabled;
    }
    bool isPBREnabled() const {
        return _pbrEnabled;
    }

private:
    
    int _frame;
    VROEyeType _eye;
    double _fps;
    bool _hdrEnabled;
    bool _pbrEnabled;
    
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
     The texture containing shadow maps for all lights. This is a depth texture array.
     */
    std::shared_ptr<VROTexture> _shadowMap;
    
    /*
     Diffuse irradiance map used for PBR image-based lighting.
     */
    std::shared_ptr<VROTexture> _irradianceMap;

    /*
     Prefiltered irradiance map used for PBR image-based specular lighting.
     */
    std::shared_ptr<VROTexture> _prefilteredMap;

    /*
     BRDF irradiance map used for PBR image-based specular lighting.
     */
    std::shared_ptr<VROTexture> _brdfMap;

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
