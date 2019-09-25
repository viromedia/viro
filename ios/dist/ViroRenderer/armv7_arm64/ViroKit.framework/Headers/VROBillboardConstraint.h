//
//  VROBillboardConstraint.h
//  ViroRenderer
//
//  Created by Raj Advani on 3/9/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROBillboardConstraint_h
#define VROBillboardConstraint_h

#include "VROConstraint.h"

enum class VROBillboardAxis {
    X,
    Y,
    Z,
    All
};

class VROBillboardConstraint : public VROConstraint {
    
public:
    
    VROBillboardConstraint(VROBillboardAxis freeAxis) :
        _freeAxis(freeAxis) {}
    
    VROMatrix4f getTransform(const VRORenderContext &context,
                                     VROMatrix4f transform);

private:
    
    VROBillboardAxis _freeAxis;
    
    VROQuaternion computeAxisRotation(VROVector3f lookAt, VROVector3f defaultAxis,
                                      VROVector3f objToCamProj);
    
};

#endif /* VROBillboardConstraint_h */
