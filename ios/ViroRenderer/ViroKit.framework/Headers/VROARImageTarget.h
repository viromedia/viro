//
//  VROARImageTarget.h
//  ViroKit
//
//  Created by Andy Chu on 1/30/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROARImageTarget_h
#define VROARImageTarget_h

#include <memory>
#include "VROARSession.h"

/*
 The orientation of the given target image.
 */
enum class VROImageOrientation {
    Up,
    Down,
    Left,
    Right
};

class VROARImageTarget {
public:
    
    VROARImageTarget() {}
    virtual ~VROARImageTarget() {}
    
    virtual void initWithTrackingImpl(VROImageTrackingImpl impl) = 0;

    void setAnchor(std::shared_ptr<VROARAnchor> anchor) {
        _anchor = anchor;
    }

    std::shared_ptr<VROARAnchor> getAnchor() {
        return _anchor.lock();
    }

private:
    // none
    std::weak_ptr<VROARAnchor> _anchor;
};

#endif /* VROARImageTarget_h */
