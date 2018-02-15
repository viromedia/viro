//
//  VROTimingFunctionCubicBezier.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/22/15.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef __VROTunedCubicBezierTimingFunction__
#define __VROTunedCubicBezierTimingFunction__

#include "VROTimingFunction.h"
#include <algorithm>

static const int kTunedBezierLookupSteps = 3000;

class VROTimingFunctionCubicBezier : public VROTimingFunction {

public:

    VROTimingFunctionCubicBezier(float x1, float y1, float x2, float y2) {
        for (int i = 0; i < kTunedBezierLookupSteps; i++) {
            float t = (float) i / (float) (kTunedBezierLookupSteps - 1);
            float t_inv = 1-t;
            float tsq = t*t;
            float t_invsq = t_inv *t_inv;
            float a = 3 * t_invsq * t;
            float b = 3 * tsq * t_inv;
            float c = tsq * t;

            xLookup[i] = x1 * a + x2 * b + c;
            yLookup[i] = y1 * a + y2 * b + c;
        }
    }

    float getT(float t) {
        float *begin = &xLookup[0];
        float *end = &xLookup[kTunedBezierLookupSteps];

        int index = std::binary_search(begin, end, t);
        if (index < 0) {
            index = 0 - (index + 1);
        }

        index = std::max(std::min(index, kTunedBezierLookupSteps - 1), 0);
        return yLookup[index];
    }

    virtual ~VROTimingFunctionCubicBezier() {}

private:

    float xLookup[kTunedBezierLookupSteps];
    float yLookup[kTunedBezierLookupSteps];

};

#endif /* defined(VROTunedCubicBezierTimingFunction__) */
