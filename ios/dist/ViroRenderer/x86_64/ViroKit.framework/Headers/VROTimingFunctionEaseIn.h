//
//  VROTimingFunctionEaseIn.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/14/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROTimingFunctionEaseIn_h
#define VROTimingFunctionEaseIn_h

#include "VROTimingFunction.h"
#include <math.h>

class VROTimingFunctionEaseIn : public VROTimingFunction {
    
public:
    
    VROTimingFunctionEaseIn() {}
    virtual ~VROTimingFunctionEaseIn() {}
    
    float getT(float t) {
        if (t <= 0.5f){
            return 2.0f * t * t;
        }
        else {
            return t;
        }
    }
    
};

#endif /* VROTimingFunctionEaseIn_h */
