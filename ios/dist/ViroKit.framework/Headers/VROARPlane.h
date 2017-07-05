//
//  VROARPlane.h
//  ViroRenderer
//
//  Created by Andy Chu on 6/17/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROARPlane_h
#define VROARPlane_h

#include "VROARPlaneAnchor.h"
#include "VROARNode.h"

/*
 This class is a nice container that associates the virtual (VRONode), the real (VROARAnchor) and
 the constraints that bind them together.
 */
class VROARPlane : public VROARNode {
public:
    VROARPlane(float minWidth, float minHeight) :
        _minWidth(minWidth),
        _minHeight(minHeight)
    {}
    
    virtual ~VROARPlane() {}
    
    /*
     Returns whether or not the given VROARAnchor fulfills this plane's requirements.
     */
    bool hasRequirementsFulfilled(std::shared_ptr<VROARAnchor> candidate) {
        std::shared_ptr<VROARPlaneAnchor> planeAnchor = std::dynamic_pointer_cast<VROARPlaneAnchor>(candidate);
        return planeAnchor && planeAnchor->getExtent().x >= _minWidth && planeAnchor->getExtent().z >= _minHeight;
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
#endif /* VROARPlane_h */
