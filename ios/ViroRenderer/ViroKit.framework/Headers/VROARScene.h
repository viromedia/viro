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
        _hasTrackingInitialized(false) {};
    virtual ~VROARScene() {};
    
    virtual void addNode(std::shared_ptr<VRONode> node);
    
    void setARComponentManager(std::shared_ptr<VROARComponentManager> arComponentManager);
    
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

    void anchorWasDetected(std::shared_ptr<VROARAnchor> anchor);
    void anchorWasUpdated(std::shared_ptr<VROARAnchor> anchor);
    void anchorWasRemoved(std::shared_ptr<VROARAnchor> anchor);
private:
    std::shared_ptr<VROARComponentManager> _arComponentManager;
    std::vector<std::shared_ptr<VROARPlaneNode>> _planes;
    std::weak_ptr<VROARSceneDelegate> _delegate;
    bool _hasTrackingInitialized;
};

#endif /* VROARScene_h */
