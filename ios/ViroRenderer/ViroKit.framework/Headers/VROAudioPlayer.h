//
//  VROAudioPlayer.h
//  ViroRenderer
//
//  Created by Raj Advani on 3/22/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROAudioPlayer_h
#define VROAudioPlayer_h

#include <Foundation/Foundation.h>
#include <AVFoundation/AVFoundation.h>

class VROAudioPlayer {
    
public:
    
    VROAudioPlayer();
    virtual ~VROAudioPlayer();
    
    void setTrack(NSURL *url, int loopCount);
    void setTrack(NSData *data, int loopCount);
    void play();
    void stop();
    void pause();
    void playIfPaused();

private:
    
    AVAudioPlayer *_player;
    bool _paused; // whether or not paused has been called
  
    void doFadeThenPause();
    void doFadeThenStop();
};

#endif /* VROAudioPlayer_h */
