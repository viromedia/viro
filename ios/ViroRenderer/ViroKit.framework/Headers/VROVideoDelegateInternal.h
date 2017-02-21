//
//  VROVideoDelegateInternal.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/10/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROVideoDelegateInternal_h
#define VROVideoDelegateInternal_h

class VROVideoDelegateInternal {
    
public:
    
    VROVideoDelegateInternal() {}
    virtual ~VROVideoDelegateInternal() {}
    
    virtual void videoDidFinish() = 0;
    virtual void onVideoUpdatedTime(int currentTimeInSeconds, int totalTimeInSeconds) = 0;
};

#endif /* VROVideoDelegateInternal_h */
