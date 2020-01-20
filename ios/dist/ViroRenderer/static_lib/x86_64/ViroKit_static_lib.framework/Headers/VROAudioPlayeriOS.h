//
//  VROAudioPlayeriOS.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/6/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

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
  
    /*
     Calls Specific to AVAudioPlayer
     */
    void play(double atTime);
    void setLoop(bool loop, int numberOfLoops);
    double getAudioDuration();
    double getDeviceCurrentTime();

#pragma mark VROSoundDataDelegate Implementation
    
    void dataIsReady();
    void dataError(std::string error);
    
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
    int _numberOfLoops;
    
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
};

#endif /* VROAudioPlayeriOS_h */
