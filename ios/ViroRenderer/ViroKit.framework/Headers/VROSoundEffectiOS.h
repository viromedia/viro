//
//  VROSoundEffectiOS.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/6/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROSoundEffectiOS_h
#define VROSoundEffectiOS_h

#include "VROSoundEffect.h"
#include <AVFoundation/AVFoundation.h>

class VROData;

class VROSoundEffectiOS : public VROSoundEffect {
    
public:
    
    VROSoundEffectiOS(std::string url);
    VROSoundEffectiOS(std::shared_ptr<VROData> data);
    virtual ~VROSoundEffectiOS();
    
    void play();
    
private:
    
    AVAudioPlayer *_player;
    
};

#endif /* VROSoundEffectiOS_h */
