//
//  VROARPlaneNode.h
//  ViroRenderer
//
//  Created by Andy Chu on 6/17/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROARPlaneNode_h
#define VROARPlaneNode_h

#include "VROARPlaneAnchor.h"
#include "VROARDeclarativeNode.h"

/*
 This class is a nice container that associates the virtual (VRONode), the real (VROARAnchor) and
 the constraints that bind them together.
 */
class VROARDeclarativePlane : public VROARDeclarativeNode {
public:
    VROARDeclarativePlane(float minWidth, float minHeight) :
        _minWidth(minWidth),
        _minHeight(minHeight)
    {}
    
    virtual ~VROARDeclarativePlane() {}
    
    /*
     Returns whether or not the given VROARAnchor fulfills this plane's requirements.
     */
    bool hasRequirementsFulfilled(std::shared_ptr<VROARAnchor> candidate) {
        std::shared_ptr<VROARPlaneAnchor> planeAnchor = std::dynamic_pointer_cast<VROARPlaneAnchor>(candidate);
        if (planeAnchor) {
            return planeAnchor && planeAnchor->getExtent().x >= _minWidth && planeAnchor->getExtent().z >= _minHeight;
        }
        return false;
    }
    
    void setMinWidth(float minWidth) {
        _minWidth = minWidth;
    }
    float getMinWidth() {
        return _minWidth;
    }
    
    void setMinHeight(float minHeight) {
        _minHeight = minHeight;
    }
    float getMinHeight() {
        return _minHeight;
    }
    
private:
    float _minWidth;
    float _minHeight;
};
#endif /* VROARPlaneNode_h */
