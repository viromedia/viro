//
//  VROSoundEffect.h
//  ViroRenderer
//
//  Created by Raj Advani on 3/23/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROSoundEffect_h
#define VROSoundEffect_h

#include <stdio.h>
#include <string>

class VROSoundEffect {
    
public:
    
    virtual ~VROSoundEffect() {}
    
    virtual void play() = 0;
    
};

#endif /* VROSoundEffect_h */
