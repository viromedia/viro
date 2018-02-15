//
//  VROMaterialAnimation.hpp
//  ViroRenderer
//
//  Created by Raj Advani on 2/14/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROMaterialAnimation_h
#define VROMaterialAnimation_h

#include <memory>
#include "VROLazyMaterial.h"

class VROMaterial;

/*
 Represents a single material animation.
 */
class VROMaterialAnimation {
    
public:
    
    VROMaterialAnimation(int index, std::shared_ptr<VROLazyMaterial> lazyMaterial) :
        _index(index),
        _lazyMaterial(lazyMaterial) {}
    virtual ~VROMaterialAnimation() {}
    
    int getIndex() const { return _index; }
    std::shared_ptr<VROMaterial> getMaterial() const { return _lazyMaterial->get(); }
    
private:
    
    const int _index;
    std::shared_ptr<VROLazyMaterial> _lazyMaterial;
    
};

#endif /* VROMaterialAnimation_h */
