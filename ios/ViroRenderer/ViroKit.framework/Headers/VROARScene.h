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

class VROARAnchor;
class VROARDeclarativeSession;
class VROARImperativeSession;
class VROPointCloudEmitter;

class VROARSceneDelegate {
public:
    virtual void onTrackingInitialized() = 0;
    virtual void onAmbientLightUpdate(float ambientLightIntensity, float colorTemperature) = 0;
};

class VROARScene : public VROScene {
public:
    VROARScene() :
        _hasTrackingInitialized(false),
        _displayPointCloud(false),
        _pointCloudSurfaceScale(VROVector3f(.01, .01, 1)),
        _pointCloudMaxPoints(500) {
            _pointCloudNode = std::make_shared<VRONode>();
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
     Set true to display/render the point cloud particles.
     */
    void displayPointCloud(bool display);

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
    void trackingHasInitialized();
    void updateAmbientLight(float intensity, float colorTemperature);
    
    void willAppear();
    void willDisappear();
    
    std::shared_ptr<VROARDeclarativeSession> getDeclarativeSession() {
        return _declarativeSession;
    }
    std::shared_ptr<VROARImperativeSession> getImperativeSession() {
        return _imperativeSession;
    }
    
private:
    
    std::weak_ptr<VROARSession> _arSession;
    std::weak_ptr<VRODriver> _driver;
    std::shared_ptr<VROARDeclarativeSession> _declarativeSession;
    std::shared_ptr<VROARImperativeSession> _imperativeSession;

    std::shared_ptr<VRONode> _pointCloudNode;
    std::shared_ptr<VROPointCloudEmitter> _pointCloudEmitter;
    std::weak_ptr<VROARSceneDelegate> _delegate;
    bool _hasTrackingInitialized;

    /* Point Cloud Properties */
    bool _displayPointCloud;
    std::shared_ptr<VROSurface> _pointCloudSurface;
    VROVector3f _pointCloudSurfaceScale;
    int _pointCloudMaxPoints;

    /*
     Creates an instance of VROPointCloudEmitter if possible.
     */
    std::shared_ptr<VROPointCloudEmitter> createPointCloudEmitter();
};

#endif /* VROARScene_h */
