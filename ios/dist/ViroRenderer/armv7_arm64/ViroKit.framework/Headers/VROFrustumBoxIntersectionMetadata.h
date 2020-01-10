//
//  VROFrustumBoxIntersectionMetadata.hpp
//  ViroRenderer
//
//  Created by Raj Advani on 4/17/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

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
