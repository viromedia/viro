//
//  VROARDeclarativeImageNode.h
//  ViroKit
//
//  Created by Andy Chu on 1/30/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROARDeclarativeImageNode_h
#define VROARDeclarativeImageNode_h

#include "VROARImageAnchor.h"
#include "VROARDeclarativeNode.h"
#include "VROARImageTarget.h"

class VROARDeclarativeImageNode : public VROARDeclarativeNode {
public:

    VROARDeclarativeImageNode() {}
    virtual ~VROARDeclarativeImageNode() {}
    
    /*
     Returns whether or not the given VROARAnchor fulfills this image marker's requirements.
     */
    bool hasRequirementsFulfilled(std::shared_ptr<VROARAnchor> candidate) {
        std::shared_ptr<VROARImageAnchor> imageAnchor = std::dynamic_pointer_cast<VROARImageAnchor>(candidate->getAnchorForTrackable());
        // a VROARImageAnchor matches an VROAR*ImageNode if they share the same VROImageTarget
        return imageAnchor && _imageTarget == imageAnchor->getImageTarget();
    }

    void setImageTarget(std::shared_ptr<VROARImageTarget> imageTarget) {
        _imageTarget = imageTarget;
    }

    std::shared_ptr<VROARImageTarget> getImageTarget() {
        return _imageTarget;
    }
    
private:
    std::shared_ptr<VROARImageTarget> _imageTarget;
};

#endif /* VROARDeclarativeImageNode_h */
