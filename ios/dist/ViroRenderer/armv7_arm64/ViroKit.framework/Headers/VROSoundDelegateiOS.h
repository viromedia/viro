//
//  VROSoundDelegateiOS.h
//  ViroRenderer
//
//  Created by Andy Chu on 1/27/17.
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
    
    void soundDidFail(std::string error) {
        [_delegate soundDidFail:[NSString stringWithUTF8String:error.c_str()]];
    }

    void soundDidFinish() {
        [_delegate soundDidFinish];
    }
private:
    __weak id <VROSoundDelegate> _delegate;

};

class VROSoundDataDelegateiOS : public VROSoundDataDelegate {
public:
    VROSoundDataDelegateiOS(std::function<void(bool success, std::string msg)> callback){
        _callback = callback;
    }
    virtual ~VROSoundDataDelegateiOS() {}

    void dataIsReady() {
        std::string emptyString;
        _callback(true, emptyString);
    }

    void dataError(std::string error) {
        _callback(false, error);
    }

private:
    std::function<void(bool success, std::string msg)> _callback;
    
};
#endif /* VROSoundDelegateiOS_h */
