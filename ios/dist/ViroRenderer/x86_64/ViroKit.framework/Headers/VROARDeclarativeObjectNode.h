//
//  VROARDeclarativeObjectNode.h
//  ViroKit
//
//  Created by Andy Chu on 8/8/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROARDeclarativeObjectNode_h
#define VROARDeclarativeObjectNode_h

#include "VROARObjectAnchor.h"
#include "VROARDeclarativeNode.h"
#include "VROARObjectTarget.h"

class VROARDeclarativeObjectNode : public VROARDeclarativeNode {
public:
    
    VROARDeclarativeObjectNode() {}
    virtual ~VROARDeclarativeObjectNode() {}
    
    /*
     Returns whether or not the given VROARAnchor fulfills this image marker's requirements.
     */
    bool hasRequirementsFulfilled(std::shared_ptr<VROARAnchor> candidate) {
        std::shared_ptr<VROARObjectAnchor> objectAnchor = std::dynamic_pointer_cast<VROARObjectAnchor>(candidate->getAnchorForTrackable());
        // a VROARObjectAnchor matches an VROAR*ObjectNode if they share the same VROARObjectTarget
        return objectAnchor && _objectTarget == objectAnchor->getObjectTarget();
    }
    
    void setObjectTarget(std::shared_ptr<VROARObjectTarget> objectTarget) {
        _objectTarget = objectTarget;
    }
    
    std::shared_ptr<VROARObjectTarget> getObjectTarget() {
        return _objectTarget;
    }
    
private:
    std::shared_ptr<VROARObjectTarget> _objectTarget;
};

#endif /* VROARDeclarativeObjectNode_h */
