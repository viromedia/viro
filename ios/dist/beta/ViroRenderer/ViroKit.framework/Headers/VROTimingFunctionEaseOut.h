//
//  VROTimingFunctionEaseOut.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/22/15.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef __VROEaseOutTimingFunction__
#define __VROEaseOutTimingFunction__

#include "VROTimingFunction.h"

class VROTimingFunctionEaseOut : public VROTimingFunction {

public:

    VROTimingFunctionEaseOut() {}
    virtual ~VROTimingFunctionEaseOut() {}

    float getT(float t) {
        if (t <= 0.5f){
            return t;
        }
        else {
            t -= 0.5f;
            return 2.0f * t * (1.0f - t) + 0.5;
        }
    }
    
};

#endif /* defined(__VROEaseOutTimingFunction__) */
