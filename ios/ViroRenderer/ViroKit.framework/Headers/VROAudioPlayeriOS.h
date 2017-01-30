//
//  VROAudioPlayeriOS.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/6/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROAudioPlayeriOS_h
#define VROAudioPlayeriOS_h

#include "VROAudioPlayer.h"
#include <AVFoundation/AVFoundation.h>

class VROAudioPlayeriOS : public VROAudioPlayer {
    
public:
    
    VROAudioPlayeriOS(std::string url, bool isLocalUrl);
    VROAudioPlayeriOS(std::shared_ptr<VROData> data);
    virtual ~VROAudioPlayeriOS();
    
    void setLoop(bool loop);
    void play();
    void pause();
    void setVolume(float volume);
    
    void setMuted(bool muted);
    void seekToTime(float seconds);
    
private:
    
    AVAudioPlayer *_player;
    float _playVolume;
    
    void doFadeThenPause();
    void doFadeThenStop();
    
};

#endif /* VROAudioPlayeriOS_h */
