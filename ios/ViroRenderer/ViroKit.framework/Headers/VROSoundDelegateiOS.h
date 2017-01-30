//
//  VROSoundDelegateiOS.h
//  ViroRenderer
//
//  Created by Andy Chu on 1/27/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROSoundDelegateiOS_h
#define VROSoundDelegateiOS_h

#include "VROSoundDelegateInternal.h"
#include "VROSoundDelegate.h"

class VROSoundDelegateiOS : public VROSoundDelegateInternal {
    
public:
    VROSoundDelegateiOS(id <VROSoundDelegate> delegate) :
        _delegate(delegate)
    {}
    virtual ~VROSoundDelegateiOS() {}

    void soundIsReady() {
        [_delegate soundIsReady];
    }

    void soundDidFinish() {
        [_delegate soundDidFinish];
    }
private:
    id <VROSoundDelegate> _delegate;

};


#endif /* VROSoundDelegateiOS_h */
