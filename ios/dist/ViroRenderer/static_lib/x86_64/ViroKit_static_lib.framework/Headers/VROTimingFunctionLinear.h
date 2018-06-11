//
//  VROTimingFunctionLinear.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/22/15.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROLINEARTIMINGFUNCTION_H_
#define VROLINEARTIMINGFUNCTION_H_

#include "VROTimingFunction.h"

class VROTimingFunctionLinear : public VROTimingFunction {
public:

    VROTimingFunctionLinear() {}
    virtual ~VROTimingFunctionLinear() {}

    float getT(float t) {
        return t;
    }
};

#endif /* VROLINEARTIMINGFUNCTION_H_ */
