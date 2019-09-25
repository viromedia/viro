//
//  VROARObjectTarget.h
//  ViroKit
//
//  Created by Andy Chu on 8/8/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROARObjectTarget_h
#define VROARObjectTarget_h

#include <memory>
#include "VROARAnchor.h"

class VROARObjectTarget {
public:
    VROARObjectTarget() {}

    virtual ~VROARObjectTarget() {}

    void setAnchor(std::shared_ptr<VROARAnchor> anchor) {
        _anchor = anchor;
    }

    std::shared_ptr<VROARAnchor> getAnchor() {
        return _anchor.lock();
    }

private:
    std::weak_ptr<VROARAnchor> _anchor;

};

#endif /* VROARObjectTarget_h */
