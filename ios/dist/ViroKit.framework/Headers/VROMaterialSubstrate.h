//
//  VROMaterialSubstrate.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/29/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROMaterialSubstrate_h
#define VROMaterialSubstrate_h

#include <stdio.h>
#include <vector>
#include <memory>

class VROSortKey;
class VROLight;
class VRORenderContext;
class VRODriver;

class VROMaterialSubstrate {
public:
    virtual ~VROMaterialSubstrate() {}
    virtual void updateSortKey(VROSortKey &key) const = 0;
    
    virtual void bindShader() = 0;
    virtual void bindLights(int lightsHash,
                            const std::vector<std::shared_ptr<VROLight>> &lights,
                            const VRORenderContext &context,
                            std::shared_ptr<VRODriver> &driver) = 0;
    
};

#endif /* VROMaterialSubstrate_h */
