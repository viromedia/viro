//
//  VROARImageAnchor.h
//  ViroKit
//
//  Created by Andy Chu on 1/30/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROARImageAnchor_h
#define VROARImageAnchor_h

#include "VROARAnchor.h"
#include "VROARImageTarget.h"

class VROARImageAnchor : public VROARAnchor {
    
public:
    
    VROARImageAnchor(std::shared_ptr<VROARImageTarget> imageTarget) :
        _imageTarget(imageTarget) {}
    virtual ~VROARImageAnchor() {}
    
    std::shared_ptr<VROARImageTarget> getImageTarget() {
        return _imageTarget;
    }
    
private:
    std::shared_ptr<VROARImageTarget> _imageTarget;
    
};

#endif /* VROARImageAnchor_h */
