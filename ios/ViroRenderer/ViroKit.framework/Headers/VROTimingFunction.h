//
//  VROTimingFunction.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/22/15.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROTIMINGFUNCTION_H_
#define VROTIMINGFUNCTION_H_

#include <memory>

enum class VROTimingFunctionType {
    Linear,
    EaseIn,
    EaseOut,
    EaseInEaseOut,
    Bounce,
    PowerDecel
};

class VROTimingFunction {
    
public:
    
    static std::unique_ptr<VROTimingFunction> forType(VROTimingFunctionType type);

    VROTimingFunction() {}
    virtual ~VROTimingFunction() {}

    virtual float getT(float t) = 0;
    
};

#endif /* VROTIMINGFUNCTION_H_ */
