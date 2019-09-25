//
//  VROARObjectAnchor.h
//  ViroKit
//
//  Created by Andy Chu on 8/8/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROARObjectAnchor_h
#define VROARObjectAnchor_h

#include "VROARAnchor.h"
#include "VROARObjectTarget.h"

class VROARObjectAnchor : public VROARAnchor {
public:
    VROARObjectAnchor(std::shared_ptr<VROARObjectTarget> objectTarget) :
        _objectTarget(objectTarget) {}

    virtual ~VROARObjectAnchor() {}

    std::shared_ptr<VROARObjectTarget> getObjectTarget() {
        return _objectTarget;
    }
private:
    std::shared_ptr<VROARObjectTarget> _objectTarget;
};

#endif /* VROARObjectAnchor_h */
