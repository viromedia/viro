//
//  VROFrameSynchronizer.h
//  ViroRenderer
//
//  Created by Raj Advani on 4/27/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROFrameSynchronizer_h
#define VROFrameSynchronizer_h

#include <memory>

class VROFrameListener;

class VROFrameSynchronizer {
    
public:
    
    virtual ~VROFrameSynchronizer() {}
    
    virtual void addFrameListener(std::shared_ptr<VROFrameListener> listener) = 0;
    virtual void removeFrameListener(std::shared_ptr<VROFrameListener> listener) = 0;
    
};

#endif /* VROFrameSynchronizer_h */
