//
//  VROSoundDelegateInternal.h
//  ViroRenderer
//
//  Created by Andy Chu on 1/20/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROSoundDelegateInternal_h
#define VROSoundDelegateInternal_h

class VROSoundDelegateInternal {

public:

    VROSoundDelegateInternal() {}
    virtual ~VROSoundDelegateInternal() {}

    // Sound load (of fail) callbacks
    virtual void soundIsReady() = 0;
    virtual void soundDidFail(std::string error) = 0;

    // Currently this is only called by the VROAudioPlayers which back the Normal VROSounds. If/when
    // GVR supports notifying us that the sounds have finished, we can change this behavior (VIRO-758)
    virtual void soundDidFinish() = 0;

};

#endif /* VROSoundDelegateInternal_h */
