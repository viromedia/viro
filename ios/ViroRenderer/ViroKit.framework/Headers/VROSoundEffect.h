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
#include <AVFoundation/AVFoundation.h>

class VROSoundEffect {
    
public:
    
    VROSoundEffect(NSURL *url);
    VROSoundEffect(NSData *data);
    virtual ~VROSoundEffect();
    
    void play();
    
private:
    
    AVAudioPlayer *_player;
    
};

#endif /* VROSoundEffect_h */
