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
    VROAudioPlayeriOS(std::shared_ptr<VROSoundData> data);
    virtual ~VROAudioPlayeriOS();
    
    /*
     Must be invoke after construction, after setting the delegate.
     */
    void setup();
    void setDelegate(std::shared_ptr<VROSoundDelegateInternal> delegate);
    
    void setLoop(bool loop);
    void play();
    void pause();
    void setVolume(float volume);
    void setMuted(bool muted);
    void seekToTime(float seconds);
  
#pragma mark VROSoundDataDelegate Implementation
    
    void dataIsReady();
    void dataError();
    
private:
    
    /*
     Underlying iOS audio player. The delegate is only kept here so that
     it's retained.
     */
    AVAudioPlayer *_player;
    VROAudioPlayerDelegate *_audioDelegate;
    
    /*
     Generic settings.
     */
    float _playVolume;
    bool _muted;
    bool _paused;
    bool _loop;
    bool _isLocal;
    
    /*
     Source audio.
     */
    std::string _url;
    std::shared_ptr<VROSoundData> _data;

    /*
     Update the underlying iOS player with the various properties set on this
     player (e.g. muted, loop, volume, etc.)
     */
    void updatePlayerProperties();

    void doFadeThenPause();
    void doFadeThenStop();
    
};

#endif /* VROAudioPlayeriOS_h */
