//
//  VROFrustum.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/15/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

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
    explicit VROFrustum(bool tdf);
    virtual ~VROFrustum();

    /*
     Generic initialization.
     */
    void fitToModelView(const double *view, const float *projection,
                        const float bufferSides, const float bufferNear, const float bufferFar);

    /*
     TDF initialization.
     */
    void fitToFrustum(const VROFrustum &source, const VROVector3f &distance);

    /*
     Intersection testing (automatically chooses best method).
     */
    VROFrustumResult intersect(VROBoundingBox *box, VROFrustumBoxIntersectionMetadata *metadata) const;

    /*
     Intersect this frustum with a bounding box, utilizing the far points optimization and
     temporal coherency optimizations.
     */
    VROFrustumResult intersectGeneric(VROBoundingBox *box, VROFrustumBoxIntersectionMetadata *metadata) const;

    /*
     Intersect this frustum with a bounding box, utilization the far points optimization in
     conjuction with temporal and translation coherency optimizations.
     */
    VROFrustumResult intersectTDF(VROBoundingBox *box, VROFrustumBoxIntersectionMetadata *metadata) const;
    VROFrustumResult intersectTDFUnoptimized(VROBoundingBox *box, VROFrustumBoxIntersectionMetadata *metadata) const;

    /*
     Frustum intersection using the "far point" optimization. The far point optimization enables us
     to determine if there's an intersection between a frustum and an AABB using only two plane->point
     distance calculations per frustum plane.
     */
    VROFrustumResult intersectWithFarPointsOpt(VROBoundingBox *box, VROFrustumBoxIntersectionMetadata *metadata) const;

    /*
     Robust frustum intersection, using no optimizations. Faster than intersectSlow, but barely.
     */
    VROFrustumResult intersectNoOpt(VROBoundingBox *box) const;

    /*
     Robust, slow frustum interesction routine that leverages containsPoint().
     */
    VROFrustumResult intersectSlow(VROBoundingBox *box) const;

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
    VROFrustumPlane planes[6];

    /*
     TDF frustum only

     The distance of any point in this frustum from the corresponding point in
     the source non-translation frustum.
     */
    VROVector3f delta;

    /*
     TDF frustum only

     The distance, in the direction of each plane's normal, between the planes
     in this frustum and the planes in the source non-translation frustum.
     */
    float planeDeltas[6];
    
    /*
     The frame during which this camera frustum was created.
     */
    uint32_t frame;
    
    /*
     True if this is a translation-delta frustum (a frustum derived from another
     by pure translation). These frustums provide faster intersection testing.
     */
    bool tdf;

};

#endif /* VROFRUSTUM_H_ */
