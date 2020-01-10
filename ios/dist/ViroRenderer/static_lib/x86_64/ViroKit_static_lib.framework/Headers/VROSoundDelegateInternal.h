//
//  VROSoundDelegateInternal.h
//  ViroRenderer
//
//  Created by Andy Chu on 1/20/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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
