//
//  VROBoundingBox.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/15/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROBOUNDINGBOX_H_
#define VROBOUNDINGBOX_H_

#include <stdlib.h>
#include <math.h>
#include "VROVector3f.h"
#include "VROMatrix4f.h"
#include <string>
#include <vector>

class VROFrustum;

enum VROBoxPlane {
    VROBoxPlaneMinX = 0,
    VROBoxPlaneMaxX = 1,
    VROBoxPlaneMinY = 2,
    VROBoxPlaneMaxY = 3,
    VROBoxPlaneMinZ = 4,
    VROBoxPlaneMaxZ = 5
};

class VROBoundingBox {
public:

    /*
     Constructors.
     */
    VROBoundingBox();
    VROBoundingBox(const VROBoundingBox &copy);
    VROBoundingBox(float left, float right, float bottom, float top, float zmin, float zmax);
    ~VROBoundingBox();

    /*
     Ray intersection. The intersection result will be stored in *intPt. If there are multiple intersection
     points, only one will be returned.
     */
    bool intersectsRay(const VROVector3f &ray, const VROVector3f &origin, VROVector3f *intPt);

    /*
     Point containment functions. The first function checks for full containment; the remaining functions
     only check for containment along the indicated axes.
     */
    bool containsPoint(const VROVector3f &point) const;
    bool containsPointXY(const VROVector3f &point) const;
    bool containsPointXZ(const VROVector3f &point) const;
    bool containsPointYZ(const VROVector3f &point) const;

    /*
     Bounding-box to bounding-box containment and intersections.
     */
    bool containsBox(const VROBoundingBox &box) const;
    bool intersectsBox(const VROBoundingBox &box) const;

    /*
     Get the coordinates and extent of the bounding box.
     */
    float getMinX() const {
        return _planes[VROBoxPlaneMinX];
    }
    float getMaxX() const {
        return _planes[VROBoxPlaneMaxX];
    }
    float getMaxY() const {
        return _planes[VROBoxPlaneMaxY];
    }
    float getMinY() const {
        return _planes[VROBoxPlaneMinY];
    }
    float getMaxZ() const {
        return _planes[VROBoxPlaneMaxZ];
    }
    float getMinZ() const {
        return _planes[VROBoxPlaneMinZ];
    }
    float getX() const {
        return (_planes[VROBoxPlaneMaxX] + _planes[VROBoxPlaneMinX]) / 2;
    }
    float getY() const {
        return (_planes[VROBoxPlaneMaxY] + _planes[VROBoxPlaneMinY]) / 2;
    }
    float getZ() const {
        return (_planes[VROBoxPlaneMaxZ] + _planes[VROBoxPlaneMinZ]) / 2;
    }
    float getSpanX() const {
        return _planes[VROBoxPlaneMaxX] - _planes[VROBoxPlaneMinX];
    }
    float getSpanY() const {
        return _planes[VROBoxPlaneMaxY] - _planes[VROBoxPlaneMinY];
    }
    float getSpanZ() const {
        return _planes[VROBoxPlaneMaxZ] - _planes[VROBoxPlaneMinZ];
    }
    VROVector3f getCenter() const {
        return { getX(), getY(), getZ() };
    }
    VROVector3f getExtents() const {
        return { getSpanX(), getSpanY(), getSpanZ() };
    }

    /*
     Set the coordinates of the bounding box.
     */
    void setMinX(float minX) {
        _planes[VROBoxPlaneMinX] = minX;
    }
    void setMaxX(float maxX) {
        _planes[VROBoxPlaneMaxX] = maxX;
    }
    void setMinY(float minY) {
        _planes[VROBoxPlaneMinY] = minY;
    }
    void setMaxY(float maxY) {
        _planes[VROBoxPlaneMaxY] = maxY;
    }
    void setMinZ(float minZ) {
        _planes[VROBoxPlaneMinZ] = minZ;
    }
    void setMaxZ(float maxZ) {
        _planes[VROBoxPlaneMaxZ] = maxZ;
    }
    
    /*
     Frustum intersection accessors.
     */
    const float *getPlanes() const {
        return _planes;
    }
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

    /*
     Scale by a relative amount (scaleBy) or an absolute amount (expandBy). Scales about the center of the
     box.
     */
    void scaleBy(float scale);
    void expandBy(float amount);
    
    /*
     Transform this bounding box by the given matrix. Note that since this is
     an AABB box, rotations can have significant impact on the size of the box.
     */
    VROBoundingBox transform(VROMatrix4f transform) const;

    /*
     Optimized set functions.
     */
    void set(const float *dimensions);
    void set(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
    void copy(const VROBoundingBox &box);

    VROBoundingBox unionWith(const VROBoundingBox &box);

    /*
     Union this bounding box with the supplied box, updating our member
     variables to be a bounding box that contains all points we previously
     contained, and any points the supplied box contains.
     */
    void unionDestructive(const VROBoundingBox &box);

    void resetFrustumDistances();
    void center(float *center) const;

    std::string toString() const;

private:

    VROBoundingBox &operator=(const VROBoundingBox &) = delete;
    
    /*
     The [xmin, xmax, ymin, ymax, zmin, zmax] extremities of this bounding-box.
     */
    float _planes[6];
    
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

#endif /* VROBOUNDINGBOX_H_ */
