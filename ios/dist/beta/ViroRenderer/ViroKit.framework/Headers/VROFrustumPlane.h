//
//  VROFrustumPlane.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/15/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROFRUSTUMPLANE_H_
#define VROFRUSTUMPLANE_H_

#include "VROPlane.h"
#include "VROBoundingBox.h"

typedef enum {
    VROFarPointPosX = 0,
    VROFarPointPosY = 1,
    VROFarPointPosZ = 2,
    VROFarPointNegX = 3,
    VROFarPointNegY = 4,
    VROFarPointNegZ = 5
} VROFarPoint;

class VROFrustumPlane : public VROPlane {
public:
    
    VROBoxPlane farPoints[6];

    VROFrustumPlane();
    virtual ~VROFrustumPlane();

    void refreshFarPoints();

};

#endif /* VROFRUSTUMPLANE_H_ */
