//
//  VROVideoDelegateiOS.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/10/16.
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

#ifndef VROVideoDelegateiOS_h
#define VROVideoDelegateiOS_h

#include "VROVideoDelegateInternal.h"
#include "VROVideoDelegate.h"

class VROVideoDelegateiOS : public VROVideoDelegateInternal {
    
public:
    
    VROVideoDelegateiOS(id <VROVideoDelegate> delegate) :
        _delegate(delegate)
    {}
    virtual ~VROVideoDelegateiOS() {}

    void videoWillBuffer() {
        [_delegate videoWillBuffer];
    }

    void videoDidBuffer() {
        [_delegate videoDidBuffer];
    }
    
    void videoDidFinish() {
        [_delegate videoDidFinish];
    }
    void onVideoUpdatedTime(float currentTimeInSeconds, float totalTimeInSeconds){
        [_delegate videoDidUpdateTime:currentTimeInSeconds totalTimeInSeconds:totalTimeInSeconds];
    }
    void videoDidFail(std::string error) {
        [_delegate videoDidFail:[NSString stringWithUTF8String:error.c_str()]];
    }

private:
    
    __weak id <VROVideoDelegate> _delegate;
    
};

#endif /* VROVideoDelegateiOS_h */
