//
//  VROARImageAnchor.h
//  ViroKit
//
//  Created by Andy Chu on 1/30/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROARImageAnchor_h
#define VROARImageAnchor_h

#include "VROARAnchor.h"
#include "VROARImageTarget.h"

enum class VROARImageTrackingMethod {
    NotTracking = 0x1,
    Tracking = 0x10,
    LastKnownPose = 0x11,
};

class VROARImageAnchor : public VROARAnchor {
    
public:
    
    VROARImageAnchor(std::shared_ptr<VROARImageTarget> imageTarget, VROARImageTrackingMethod trackingMethod) :
        _imageTarget(imageTarget),
        _trackingMethod(trackingMethod) {}
    virtual ~VROARImageAnchor() {}
    
    std::shared_ptr<VROARImageTarget> getImageTarget() {
        return _imageTarget;
    }

    void setTrackingMethod(VROARImageTrackingMethod trackingMethod) {
        _trackingMethod = trackingMethod;
    }

    VROARImageTrackingMethod getTrackingMethod() {
        return _trackingMethod;
    }
    
private:
    std::shared_ptr<VROARImageTarget> _imageTarget;
    VROARImageTrackingMethod _trackingMethod;
    
};

#endif /* VROARImageAnchor_h */
