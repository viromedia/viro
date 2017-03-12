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
#include "VROSoundDataDelegate.h"
#include "VROSoundData.h"
#include <AVFoundation/AVFoundation.h>

/*
 Simple object that wraps a VROSoundDelegateInternal object and acts as a delegate for the AVAudioPlayer
 */
@interface VROAudioPlayerDelegate : NSObject <AVAudioPlayerDelegate>

- (id)initWithSoundDelegate:(std::shared_ptr<VROSoundDelegateInternal>)soundDelegate;

- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag;

@end

class VROAudioPlayeriOS : public VROAudioPlayer, public VROSoundDataDelegate, public std::enable_shared_from_this<VROAudioPlayeriOS> {
    
public:
    VROAudioPlayeriOS(std::string url, bool isLocalUrl);
    VROAudioPlayeriOS(std::shared_ptr<VROData> data);
    
    // Use static create over the VROSoundData constructor (so that you don't need to call setup).
    static std::shared_ptr<VROAudioPlayeriOS> create(std::shared_ptr<VROSoundData> data);
    VROAudioPlayeriOS(std::shared_ptr<VROSoundData> data);

    virtual ~VROAudioPlayeriOS();
    
    void setLoop(bool loop);
    void play();
    void pause();
    void setVolume(float volume);
  void setDelegate(std::shared_ptr<VROSoundDelegateInternal> delegate);
    void setMuted(bool muted);
    void seekToTime(float seconds);
  
#pragma mark VROSoundDataDelegate Implementation
    
    void dataIsReady();
    void dataError();
    
private:
    
    AVAudioPlayer *_player;
    VROAudioPlayerDelegate *_audioDelegate;
    float _playVolume;
    bool _muted;
    bool _paused;
    bool _loop;
    bool _isLocal;
    std::shared_ptr<VROSoundData> _data;

    /*
     Update the underlying iOS player with the various properties set on this
     player (e.g. muted, loop, volume, etc.)
     */
    void updatePlayerProperties();

    void setup();
    void doFadeThenPause();
    void doFadeThenStop();
    
};

#endif /* VROAudioPlayeriOS_h */
