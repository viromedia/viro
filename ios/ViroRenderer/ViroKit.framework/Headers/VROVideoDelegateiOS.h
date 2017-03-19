//
//  VROVideoDelegateiOS.hpp
//  ViroRenderer
//
//  Created by Raj Advani on 11/10/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

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
    
    void videoDidFinish() {
        [_delegate videoDidFinish];
    }
    void onVideoUpdatedTime(int currentTimeInSeconds, int totalTimeInSeconds){
        [_delegate videoDidUpdateTime:currentTimeInSeconds totalTimeInSeconds:totalTimeInSeconds];
    }
    void videoDidFail(std::string error) {
        [_delegate videoDidFail:[NSString stringWithUTF8String:error.c_str()]];
    }

private:
    
    __weak id <VROVideoDelegate> _delegate;
    
};

#endif /* VROVideoDelegateiOS_h */
