//
//  VROHitTestResult.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/13/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROHitTestResult_h
#define VROHitTestResult_h

#include "VROVector3f.h"
#include <memory>

class VRONode;

class VROHitTestResult {
    
public:
    
    VROHitTestResult(std::shared_ptr<VRONode> node, VROVector3f location, float distance, bool background) :
        _node(node),
        _location(location),
        _distance(distance),
        _background(background)
    {}
    
    ~VROHitTestResult() {}
    
    std::shared_ptr<VRONode> getNode() const {
        return _node;
    }
    
    VROVector3f getLocation() const {
        return _location;
    }
    
    float getDistance() const {
        return _distance;
    }
    
    bool isBackgroundHit() const {
        return _background;
    }

private:
    
    std::shared_ptr<VRONode> _node;
    VROVector3f _location;
    float _distance;
    bool _background;

};

#endif /* VROHitTestResult_h */
