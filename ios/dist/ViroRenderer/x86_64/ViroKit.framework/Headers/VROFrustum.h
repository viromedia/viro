//
//  VROFrustum.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/15/15.
//  Copyright © 2015 Viro Media. All rights reserved.
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

#ifndef VROFRUSTUM_H_
#define VROFRUSTUM_H_

#include "VROFrustumPlane.h"
#include "VROVector3f.h"
#include "VROFrustumBoxIntersectionMetadata.h"

/*
 Plane identifiers.
 */
enum class VROFrustumSide : int {
    Left = 0,
    Right = 1,
    Bottom = 2,
    Top = 3,
    Near = 4,
    Far = 5
};

/*
 Enum for intersection testing results.
 */
enum class VROFrustumResult {
    Inside,
    Intersects,
    Outside
};

class VROFrustum {
public:

    /*
     Create a new frustum.
     */
    explicit VROFrustum();
    virtual ~VROFrustum();

    /*
     Generic initialization.
     */
    void fitToModelView(const float *view, const float *projection,
                        const float bufferSides, const float bufferNear, const float bufferFar);
    
    /*
     Makes the far clipping plane infinity. This is useful if we want to compute intersections
     without taking the FCP into account (e.g. if we want an auto-adjusting FCP).
     */
    void removeFCP();

    /*
     Intersection testing (automatically chooses best method).
     */
    VROFrustumResult intersect(const VROBoundingBox &box, VROFrustumBoxIntersectionMetadata *metadata) const;

    /*
     Intersect this frustum with a bounding box, utilizing the far points optimization and
     temporal coherency optimizations.
     */
    VROFrustumResult intersectAllOpt(const VROBoundingBox &box, VROFrustumBoxIntersectionMetadata *metadata) const;

    /*
     Frustum intersection using the "far point" optimization. The far point optimization enables us
     to determine if there's an intersection between a frustum and an AABB using only two plane->point
     distance calculations per frustum plane.
     */
    VROFrustumResult intersectWithFarPointsOpt(const VROBoundingBox &box) const;

    /*
     Robust frustum intersection, using no optimizations.
     */
    VROFrustumResult intersectNoOpt(const VROBoundingBox &box) const;

    /*
     Check if the given point is contained by this frustum.
     */
    bool containsPoint(const VROVector3f &point) const;

    /*
     Get the distance of the given point from the near or far clipping plane of this
     frustum.
     */
    float distanceFromFCP(VROVector3f pt) const;
    float distanceFromNCP(VROVector3f pt) const;

private:

    /*
     The planes composing this frustum: left, right, bottom, top, near, far.
     */
    VROFrustumPlane _planes[6];
    
};

#endif /* VROFRUSTUM_H_ */
