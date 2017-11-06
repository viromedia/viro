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
        return _anchor.lock();
    }
    
    void setPauseUpdates(bool pauseUpdates);
    bool shouldPauseUpdates() {
        return _pauseUpdates;
    }
    
protected:
    
    std::weak_ptr<VROARAnchor> _anchor;
    bool _pauseUpdates;
    
};

#endif /* VROARNode_h */
