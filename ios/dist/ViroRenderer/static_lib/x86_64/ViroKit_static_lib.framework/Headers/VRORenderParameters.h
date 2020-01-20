//
//  VRORenderParameters.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/15/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

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
    
    std::stack<float> opacities;
    std::vector<std::shared_ptr<VROLight>> lights;
    std::stack<int> hierarchyDepths;
    std::stack<float> distancesFromCamera;
    int hierarchyId;
    float furthestDistanceFromCamera;
    
    VRORenderParameters() {
        opacities.push(1.0);
        hierarchyDepths.push(-1);
        hierarchyId = 0;
        furthestDistanceFromCamera = 0;
        distancesFromCamera.push(0);
    }
    
};

#endif /* VRORenderParameters_h */
