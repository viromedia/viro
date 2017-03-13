//
//  VROSoundDataDelegate.h
//  ViroRenderer
//
//  Created by Andy Chu on 1/30/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROSoundDataDelegate_h
#define VROSoundDataDelegate_h

/**
 * This is the delegate for the SoundObject
 */
class VROSoundDataDelegate {
    
public:
    
    virtual ~VROSoundDataDelegate() {}
    
    virtual void dataIsReady() = 0;
    virtual void dataError() = 0;
    
};


#endif /* VROSoundDataDelegate_h */
