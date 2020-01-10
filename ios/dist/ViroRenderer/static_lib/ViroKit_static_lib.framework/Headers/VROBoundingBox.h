//
//  VROBoundingBox.h
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
    VROBoundingBox() noexcept;
    VROBoundingBox(float left, float right, float bottom, float top, float zmin, float zmax);

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
     Distance from the point to the bounding box.
     */
    float getDistanceToPoint(VROVector3f p) const;
    
    /*
     Distance from the point to the furthest point on the bounding box.
     */
    float getFurthestDistanceToPoint(VROVector3f p) const;

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
    
    const float *getPlanes() const {
        return _planes;
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
    void center(float *center) const;

    std::string toString() const;

private:
    
    /*
     The [xmin, xmax, ymin, ymax, zmin, zmax] extremities of this bounding-box.
     */
    float _planes[6];

};

#endif /* VROBOUNDINGBOX_H_ */
