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
#include "VROARNodeDelegate.h"

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
    
    void setARNodeDelegate(std::shared_ptr<VROARNodeDelegate> delegate) {
        _arNodeDelegate = delegate;
    }
    
    std::shared_ptr<VROARNodeDelegate> getARNodeDelegate() {
        return _arNodeDelegate.lock();
    }
    
    void onARAnchorAttached() {
        std::shared_ptr<VROARNodeDelegate> delegate = getARNodeDelegate();
        if (delegate) {
            delegate->onARAnchorAttached(getAnchor());
        }
    }
    
    void onARAnchorUpdated() {
        std::shared_ptr<VROARNodeDelegate> delegate = getARNodeDelegate();
        if (delegate) {
            delegate->onARAnchorUpdated(getAnchor());
        }
    }
    
    void onARAnchorRemoved() {
        std::shared_ptr<VROARNodeDelegate> delegate = getARNodeDelegate();
        if (delegate) {
            delegate->onARAnchorRemoved();
        }
    }
    
protected:
    std::weak_ptr<VROARAnchor> _anchor;
    std::weak_ptr<VROARNodeDelegate> _arNodeDelegate;

};
#endif /* VROARNode_h */
