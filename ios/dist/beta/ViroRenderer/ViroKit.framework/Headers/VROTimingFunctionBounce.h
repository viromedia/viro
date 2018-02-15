//
//  VROTimingFunctionBounce.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/22/15.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROBOUNCETIMINGFUNCTION_H_
#define VROBOUNCETIMINGFUNCTION_H_

#include "VROTimingFunction.h"

class VROTimingFunctionBounce : public VROTimingFunction {
public:
    
    VROTimingFunctionBounce() {}
    virtual ~VROTimingFunctionBounce() {}

    float getT(float t) {
        if (t < 0.5) {
            return (float) (t / 0.45);
        }
        else if (t < 0.67) {
            return (float) ((0.5 / 0.45) - ((t - 0.5) / .85));
        }
        else {
            return (float) ((0.5 / 0.45) - ((0.67 - 0.5) / .85) + (t - 0.67) / 3.3);
        }
    }
};

#endif /* VROBOUNCETIMINGFUNCTION_H_ */
