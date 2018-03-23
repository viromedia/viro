//
//  VROARScene.h
//  ViroKit
//
//  Created by Andy Chu on 6/13/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROARScene_h
#define VROARScene_h

#include <vector>
#include "VROARSession.h"
#include "VROScene.h"
#include "VROARCamera.h"

class VROARAnchor;
class VROARDeclarativeSession;
class VROARImperativeSession;
class VROFixedParticleEmitter;

class VROARSceneDelegate {
public:
    virtual void onTrackingUpdated(VROARTrackingState state, VROARTrackingStateReason reason) = 0;
    virtual void onAmbientLightUpdate(float ambientLightIntensity, float colorTemperature) = 0;
};

class VROARScene : public VROScene {
public:
    VROARScene() :
        _currentTrackingState(VROARTrackingState::Unavailable),
        _currentTrackingStateReason(VROARTrackingStateReason::None),
        _displayPointCloud(false),
        _pointCloudSurfaceScale(VROVector3f(.01, .01, 1)),
        _pointCloudMaxPoints(500) {
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
    void updateAmbientLight(float intensity, float colorTemperature);
    
    void willAppear();
    void willDisappear();
    
    std::shared_ptr<VROARDeclarativeSession> getDeclarativeSession() {
        return _declarativeSession;
    }
    std::shared_ptr<VROARImperativeSession> getImperativeSession() {
        return _imperativeSession;
    }

    void updateParticles(const VRORenderContext &context);
private:
    std::set<VROAnchorDetection> _detectionTypes;
    
    std::weak_ptr<VROARSession> _arSession;
    std::weak_ptr<VRODriver> _driver;
    std::shared_ptr<VROARDeclarativeSession> _declarativeSession;
    std::shared_ptr<VROARImperativeSession> _imperativeSession;

    std::shared_ptr<VRONode> _pointCloudNode;
    std::shared_ptr<VROFixedParticleEmitter> _pointCloudEmitter;
    std::weak_ptr<VROARSceneDelegate> _delegate;

    /*
     * Tracking states that have been read from ARCore/ARKit cameras.
     * Usually, this flips between Unavailable and Normal as the activity
     * enters paused and resume states.
     */
    VROARTrackingState _currentTrackingState;
    VROARTrackingStateReason _currentTrackingStateReason;

    /* Point Cloud Properties */
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
