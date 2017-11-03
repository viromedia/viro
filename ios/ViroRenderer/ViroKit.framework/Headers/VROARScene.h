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
#include "VROScene.h"
#include "VROARSession.h"
#include "VROARComponentManager.h"
#include "VROPointCloudEmitter.h"

class VROARSceneDelegate {
public:
    virtual void onTrackingInitialized() = 0;
    virtual void onAmbientLightUpdate(float ambientLightIntensity, float colorTemperature) = 0;
    virtual void onAnchorFound(std::shared_ptr<VROARAnchor> anchor) = 0;
    virtual void onAnchorUpdated(std::shared_ptr<VROARAnchor> anchor) = 0;
    virtual void onAnchorRemoved(std::shared_ptr<VROARAnchor> anchor) = 0;
};

class VROARScene : public VROScene, public VROARComponentManagerDelegate {
public:
    VROARScene() :
        _hasTrackingInitialized(false),
        _displayPointCloud(false),
        _pointCloudSurfaceScale(VROVector3f(.01, .01, 1)),
        _pointCloudMaxPoints(500) {};
    virtual ~VROARScene() {};
    
    virtual void addNode(std::shared_ptr<VRONode> node);
    
    void setARComponentManager(std::shared_ptr<VROARComponentManager> arComponentManager);
    void setARSession(std::shared_ptr<VROARSession> arSession);
    void setDriver(std::shared_ptr<VRODriver> driver);

    /*
     Set true to display/render the point cloud particles.
     */
    void displayPointCloud(bool display);

    /*
     Reset the point cloud surface to the default
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
    
    // TODO: see if renaming these to addARNode would work. Also see if we even need to call
    // this independent of addNode... I don't think so.
    void addARPlane(std::shared_ptr<VROARPlaneNode> plane);
    void removeARPlane(std::shared_ptr<VROARPlaneNode> plane);
    void updateARPlane(std::shared_ptr<VROARPlaneNode> plane);

    // VROARComponentManagerDelegate functions
    void anchorWasDetected(std::shared_ptr<VROARAnchor> anchor);
    void anchorWasUpdated(std::shared_ptr<VROARAnchor> anchor);
    void anchorWasRemoved(std::shared_ptr<VROARAnchor> anchor);
private:
    std::shared_ptr<VROARComponentManager> _arComponentManager;
    std::weak_ptr<VROARSession> _arSession;
    std::weak_ptr<VRODriver> _driver;

    std::shared_ptr<VROPointCloudEmitter> _pointCloudEmitter;
    std::vector<std::shared_ptr<VROARPlaneNode>> _planes;
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
