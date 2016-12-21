//
//  VROHoverController.h
//  ViroRenderer
//
//  Created by Raj Advani on 2/7/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROHoverController_h
#define VROHoverController_h

#include "VROFrameListener.h"
#include "VROQuaternion.h"
#include <functional>
#include <memory>

class VROScene;
class VRONode;
class VROHoverDelegate;
class VROHoverDistanceListener;

class VROHoverController : public VROFrameListener {
    
public:
    
    VROHoverController(float rotationThresholdRadians,
                       std::shared_ptr<VROScene> scene);
    virtual ~VROHoverController();
    
    void setDelegate(std::shared_ptr<VROHoverDelegate> delegate);
    
    void addHoverDistanceListener(std::shared_ptr<VROHoverDistanceListener> listener);
    void removeHoverDistanceListener(std::shared_ptr<VROHoverDistanceListener> listener);
    
    void onFrameWillRender(const VRORenderContext &context);
    void onFrameDidRender(const VRORenderContext &context);
    
private:
    
    std::weak_ptr<VROScene> _scene;
    std::weak_ptr<VRONode> _hoveredNode;
    std::weak_ptr<VROHoverDelegate> _delegate;
    std::vector<std::shared_ptr<VROHoverDistanceListener>> _distanceListeners;
    bool _firstHoverEvent;
    
    const float _rotationThresholdRadians;
    VROVector3f _lastCameraForward;

    std::shared_ptr<VRONode> findHoveredNode(VROVector3f ray, std::shared_ptr<VROScene> &scene,
                         const VRORenderContext &context);
    void notifyDelegatesOnHoveredNode(std::shared_ptr<VRONode> node);
};

#endif /* VROHoverController_h */
