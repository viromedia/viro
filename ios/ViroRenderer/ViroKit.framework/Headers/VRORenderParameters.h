//
//  VRORenderParameters.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/15/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VRORenderParameters_h
#define VRORenderParameters_h

#include <vector>
#include <stack>
#include "VROMatrix4f.h"

class VROLight;

/*
 Contains the per-frame render parameters for the current
 render pass.
 */
class VRORenderParameters {
    
public:
    
    std::stack<VROMatrix4f> transforms;
    std::stack<float> opacities;
    std::vector<std::shared_ptr<VROLight>> lights;
    std::stack<int> hierarchyDepths;
    std::stack<float> distancesFromCamera;
    int hierarchyId;
    
    VRORenderParameters() {
        opacities.push(1.0);
        hierarchyDepths.push(-1);
        hierarchyId = 0;
        distancesFromCamera.push(0);
    }
    
};

#endif /* VRORenderParameters_h */
