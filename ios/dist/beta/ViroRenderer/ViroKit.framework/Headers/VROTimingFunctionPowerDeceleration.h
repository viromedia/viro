//
//  VROTimingFunctionPowerDeceleration.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/22/15.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef __VROPowerDecelerationTimingFunction__
#define __VROPowerDecelerationTimingFunction__

#include "VROTimingFunction.h"
#include "VROLog.h"

/*
 The graph goes from (0,0) smoothly to (1,1).
 The slope of x=1 is 0, meaning it smoothly comes to a stop.
 The slope of x=0 is the first derivative of y=x^e for x=1
 (since this function is y=x^e flipped both horizontally and
 vertically, such that the (0,0) point is the (1,1) point of
 y=x^e), which is y'(1)=e, the exponent itself!

 E.g.
 For y=x^2, the slope at x=1 is 2.
 For y=x^3, the slope at x=1 is 3.
 etc.

 1<x<2: calm slowdown, but abrupt stop.
 x=2: standard.
 x>2: quicker slowdown, stop is more graceful.
 Note that this is also the slope of the "entry" point at (0,0).
 */
static const double kPowerFunctionDecelerationExponent = 2.0;

class VROTimingFunctionPowerDeceleration : public VROTimingFunction {

public:

    VROTimingFunctionPowerDeceleration() {}
    virtual ~VROTimingFunctionPowerDeceleration() {}

    float getT(float t) {
        passert (t >= 0 && t <= 1);
        return (float) (1 - pow(1 - t, kPowerFunctionDecelerationExponent));
    }
    
};

#endif /* defined(__VROPowerDecelerationTimingFunction__) */
