//
//  VROLazyMaterial.h
//  ViroRenderer
//
//  Created by Raj Advani on 2/14/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROLazyMaterial_h
#define VROLazyMaterial_h

#include <memory>

class VROMaterial;

/*
 Interface for a lazy-loading material. Enables the animation system
 to retrieve target materials lazily at the start of material animations.
 */
class VROLazyMaterial {

public:
    
    virtual std::shared_ptr<VROMaterial> get() = 0;
    
};

#endif /* VROLazyMaterial_h */
