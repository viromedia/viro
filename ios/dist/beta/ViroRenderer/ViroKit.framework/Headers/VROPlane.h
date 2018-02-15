//
//  VROPlane.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/15/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROPLANE_H_
#define VROPLANE_H_

#include "VROVector3f.h"
#include "VROQuaternion.h"

enum class VROPlaneHalfSpace {
    OnPlane,
    Negative,
    Positive
};

class VROPlane {
public:

    /*
     The normal of the plane (defines a,b,c in the plane equation).
     */
    VROVector3f normal;

    /*
     The distance of the plane from the origin in the direction of its normal.
     */
    float d;

    VROPlane();
    VROPlane(VROVector3f normal, float d);
    VROPlane(VROVector3f normal, VROVector3f point);
    virtual ~VROPlane();

    VROVector3f projectNormalizedVector(VROVector3f vector);

    float distanceToPoint(VROVector3f point) const;
    float distanceToPointXY(VROVector3f point) const;

    VROPlaneHalfSpace getHalfSpaceOfPoint(VROVector3f point) const;

    float getA() const { return normal.x; }
    float getB() const { return normal.y; }
    float getC() const { return normal.z; }
    float getD() const { return d; }

    void setA(float a) { normal.x = a; }
    void setB(float b) { normal.y = b; }
    void setC(float c) { normal.z = c; }
    void setD(float d) { this->d = d; }

    void normalize();
};

#endif /* VROPLANE_H_ */
