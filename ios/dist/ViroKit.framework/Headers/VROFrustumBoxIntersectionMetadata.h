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
    
    void reset();
    
    float *getPositiveDistanceFromPlanes() {
        return _positiveDistanceFromPlanes;
    }
    float *getNegativeDistanceFromPlanes() {
        return _negativeDistanceFromPlanes;
    }
    
    const VROFrustum *getSourceFrustumForDistances() const {
        return _sourceFrustumForDistances;
    }
    void setSourceFrustumForDistances(const VROFrustum *frustum) {
        _sourceFrustumForDistances = frustum;
    }
    
    uint32_t getDistanceFrame() const {
        return _distanceFrame;
    }
    void setDistanceFrame(uint32_t frame) {
        _distanceFrame = frame;
    }
    
    uint8_t getPlaneLastOutside() const {
        return _planeLastOutside;
    }
    void setPlaneLastOutside(uint8_t plane) {
        _planeLastOutside = plane;
    }
    
    void setFrustumDistanceValid(bool valid) {
        _distancesValid = valid;
    }
    bool isFrustumDistanceValid() const {
        return _distancesValid;
    }
    
private:
    
    /*
     The frame and frustum for which the positive and negative distances were last recorded.
     */
    const VROFrustum *_sourceFrustumForDistances;
    uint32_t _distanceFrame;
    
    /*
     Distance of the positive far points of this box from each of the planes of the
     view-frustum during the last render-cycle.
     */
    float _positiveDistanceFromPlanes[6];
    
    /*
     Distance of the negative far points of this box from each of the planes of the
     view-frustum during the last render-cycle.
     */
    float _negativeDistanceFromPlanes[6];
    
    /*
     The plane of the frustum this bounding-box was last outside.
     */
    uint8_t _planeLastOutside;
    
    /*
     True if the positive and negative distances have been recorded.
     */
    bool _distancesValid;
    
};

#endif /* VROFrustumBoxIntersectionMetadata_h */
