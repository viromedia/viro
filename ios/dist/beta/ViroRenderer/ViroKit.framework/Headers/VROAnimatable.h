//
//  VROAnimatable.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/17/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROAnimatable_h
#define VROAnimatable_h

#include <stdio.h>
#include <string>
#include "VROVector3f.h"
#include <functional>
#include <memory>

class VROAnimation;

/*
 Marker class for objects that have animatable properties.
 */
class VROAnimatable : public std::enable_shared_from_this<VROAnimatable> {
public:
    
    void animate(std::shared_ptr<VROAnimation> animation);

    virtual void onAnimationFinished(){
        //No-op
    }
};

#endif /* VROAnimatable_hpp */