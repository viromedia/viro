//
//  VROFrustumBoxIntersectionMetadata.hpp
//  ViroRenderer
//
//  Created by Raj Advani on 4/17/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROFrustumBoxIntersectionMetadata_h
#define VROFrustumBoxIntersectionMetadata_h

#include <math.h>
#include <stdint.h>

class VROFrustum;

/*
 Stores metadata about the intersection between a given VROFrustum
 and VROBoundingBox, which can be utilized to accelerate intersection
 computations between the same frustum and box in subsequent 
 frames.
 */
class VROFrustumBoxIntersectionMetadata {
    
public:
    
    VROFrustumBoxIntersectionMetadata();
    ~VROFrustumBoxIntersectionMetadata();
    
    uint8_t getPlaneLastOutside() const {
        return _planeLastOutside;
    }
    void setPlaneLastOutside(uint8_t plane) {
        _planeLastOutside = plane;
    }
    
private:
    
    /*
     The plane of the frustum the bounding-box was last outside.
     */
    uint8_t _planeLastOutside;
    
};

#endif /* VROFrustumBoxIntersectionMetadata_h */
