//
//  VROARNode.h
//  ViroKit
//
//  Created by Andy Chu on 6/16/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROARNode_h
#define VROARNode_h

#include "VRONode.h"

class VROARAnchor;

class VROARNode : public VRONode {
public:
    VROARNode() {}
    virtual ~VROARNode() {}
    
    bool hasRequirementFulfilled(std::shared_ptr<VROARAnchor> candidate) {
        return false;
    };
    
    void setAnchor(std::shared_ptr<VROARAnchor> anchor) {
        _anchor = anchor;
    }
    
    std::shared_ptr<VROARAnchor> getAnchor() {
        return _anchor.lock();
    }
    
protected:
    std::weak_ptr<VROARAnchor> _anchor;

};
#endif /* VROARNode_h */
