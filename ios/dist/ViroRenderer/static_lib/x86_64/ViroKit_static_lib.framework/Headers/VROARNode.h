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

/*
 ARNode is a specialized Node that corresponds to a detected ARAnchor. Each ARNode is
 continually updated to stay in sync with its corresponding ARAnchor: if the anchor's
 position, orientation, or other detected properties change, the ARNode will be changed
 as well.
 */
class VROARNode : public VRONode {
public:

    VROARNode() : _pauseUpdates(false) {}
    virtual ~VROARNode() {}
    
    void setAnchor(std::shared_ptr<VROARAnchor> anchor) {
        _anchor = anchor;
    }
    std::shared_ptr<VROARAnchor> getAnchor() {
        return _anchor;
    }
    
    void setPauseUpdates(bool pauseUpdates);
    bool shouldPauseUpdates() {
        return _pauseUpdates;
    }
    
protected:

    /*
     There is an *intentional* strong reference cycle between VROARNode and VROARAnchor. Anchors and
     ARNodes are managed in one of two ways:

     1. By the AR subsystem (ARCore). When anchors are bound to trackables, and the bound trackable
        disappears, VROARSessionARCore will remove the corresponding VROARAnchor and its VROARNode.

     2. Manually, by attaching anchors to hit results. In this case, anchors and nodes are removed
        together when the ARNode is detached from the system (see ARNode.detach() in Java).
     */
    std::shared_ptr<VROARAnchor> _anchor;
    bool _pauseUpdates;
    
};

#endif /* VROARNode_h */
