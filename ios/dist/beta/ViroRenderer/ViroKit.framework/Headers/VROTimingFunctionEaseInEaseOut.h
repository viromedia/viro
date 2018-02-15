//
//  VROTimingFunctionEaseInEaseOut.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/22/15.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef __VROEaseInEaseOutTimingFunction__
#define __VROEaseInEaseOutTimingFunction__

#include "VROTimingFunction.h"
#include <math.h>

class VROTimingFunctionEaseInEaseOut : public VROTimingFunction {

public:

    VROTimingFunctionEaseInEaseOut() {}
    virtual ~VROTimingFunctionEaseInEaseOut() {}

    float getT(float t) {
        if (t <= 0.5f){
            return 2.0f * t * t;
        }
        else {
            t -= 0.5f;
            return 2.0f * t * (1.0f - t) + 0.5;
        }
    }

};

#endif /* defined(__VROEaseInEaseOutTimingFunction__) */
