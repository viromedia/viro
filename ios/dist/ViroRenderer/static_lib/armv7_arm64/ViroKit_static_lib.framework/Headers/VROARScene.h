//
//  VROARScene.h
//  ViroKit
//
//  Created by Andy Chu on 6/13/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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

#ifndef VROARScene_h
#define VROARScene_h

#include <vector>
#include "VROARSession.h"
#include "VROScene.h"
#include "VROAtomic.h"
#include "VROARCamera.h"

class VROARAnchor;
class VROARDeclarativeSession;
class VROARImperativeSession;
class VROFixedParticleEmitter;

class VROARSceneDelegate {
public:
    virtual void onTrackingUpdated(VROARTrackingState state, VROARTrackingStateReason reason) = 0;
    virtual void onAmbientLightUpdate(float ambientLightIntensity, VROVector3f ambientLightColor) = 0;
};

class VROARScene : public VROScene {
public:
    VROARScene() :
        _currentTrackingState(VROARTrackingState::Unavailable),
        _currentTrackingStateReason(VROARTrackingStateReason::None),
        _displayPointCloud(false),
        _pointCloudSurfaceScale(VROVector3f(.01, .01, 1)),
        _pointCloudMaxPoints(500) {
            _ambientLightIntensity = 1000;
            _ambientLightColor = { 1.0, 1.0, 1.0 };
            _pointCloudNode = std::make_shared<VRONode>();
            _pointCloudEmitter = nullptr;
            _detectionTypes = { VROAnchorDetection::PlanesHorizontal }; //default is horizontal
        };
    virtual ~VROARScene();
    
    void initDeclarativeSession();
    void initImperativeSession();
    std::shared_ptr<VROARSessionDelegate> getSessionDelegate();
    
    void setARSession(std::shared_ptr<VROARSession> arSession);
    void setDriver(std::shared_ptr<VRODriver> driver);

    /*
     Add AR nodes. These are directly added to the root node.
     */
    void addNode(std::shared_ptr<VRONode> node);

    /*
     The set of anchor detection types we want to enable for this scene.
     */
    void setAnchorDetectionTypes(std::set<VROAnchorDetection> detectionTypes);

    /*
     Set true to display/render the point cloud particles.
     */
    void displayPointCloud(bool display);

    /*
     Update our point cloud particles with the latest positional data received from the last
     ar frame.
     */
    void updatePointCloud();

    /*
     Reset the point cloud surface to the default.
     */
    void resetPointCloudSurface();

    /*
     Set the surface to use for the individual point cloud particles.
     */
    void setPointCloudSurface(std::shared_ptr<VROSurface> surface);

    /*
     Set the scale of the individual point cloud particles.
     */
    void setPointCloudSurfaceScale(VROVector3f scale);

    /*
     Sets the max number of point cloud points to display/render at any one time.
     */
    void setPointCloudMaxPoints(int maxPoints);
    
    void setDelegate(std::shared_ptr<VROARSceneDelegate> delegate);
    void setTrackingState(VROARTrackingState state, VROARTrackingStateReason reason, bool force);

    /*
     Get the ambient light estimate of the scene.
     */
    float getAmbientLightIntensity() const {
        return _ambientLightIntensity;
    }
    VROVector3f getAmbientLightColor() const {
        return _ambientLightColor;
    }
    
    /*
     Update the ambient light estimate of the scene. Intensity is reported in
     lumens and color is reported in linear space.
     */
    void updateAmbientLight(float intensity, VROVector3f color);
    
    void willAppear();
    void willDisappear();

    std::shared_ptr<VROARSession> getARSession() {
        return _arSession.lock();
    }
    std::shared_ptr<VROARDeclarativeSession> getDeclarativeSession() {
        return _declarativeSession;
    }
    std::shared_ptr<VROARImperativeSession> getImperativeSession() {
        return _imperativeSession;
    }

    void updateParticles(const VRORenderContext &context);
    
private:
    
    /*
     The anchors that we should detect in this scene. This is the *ground truth* setting for
     this variable. It is stored here so that it can be pushed into the VROARSession when that
     is injected into the scene.
     */
    std::set<VROAnchorDetection> _detectionTypes;
    
    std::weak_ptr<VROARSession> _arSession;
    std::weak_ptr<VRODriver> _driver;
    std::shared_ptr<VROARDeclarativeSession> _declarativeSession;
    std::shared_ptr<VROARImperativeSession> _imperativeSession;

    std::shared_ptr<VRONode> _pointCloudNode;
    std::shared_ptr<VROFixedParticleEmitter> _pointCloudEmitter;
    std::weak_ptr<VROARSceneDelegate> _delegate;
    
    /*
     Ambient light estimation. The intensity is in lumens and the color is in linear space.
     */
    VROAtomic<float> _ambientLightIntensity;
    VROAtomic<VROVector3f> _ambientLightColor;

    /*
     Tracking states that have been read from AR cameras. Usually, this flips between Unavailable
     and Normal as the activity enters paused and resumed states, or between Limited and Normal
     amid changing environmental conditions.
     */
    VROARTrackingState _currentTrackingState;
    VROARTrackingStateReason _currentTrackingStateReason;

    /*
     Point cloud properties.
     */
    bool _displayPointCloud;
    std::shared_ptr<VROSurface> _pointCloudSurface = nullptr;
    VROVector3f _pointCloudSurfaceScale;
    int _pointCloudMaxPoints;

    /*
     Creates an instance of VROFixedParticleEmitter representing the point cloud, if possible.
     */
    void initPointCloudEmitter();
};

#endif /* VROARScene_h */
