//
//  VROProjector.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/13/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROProjector_h
#define VROProjector_h

#include "VROVector3f.h"

class VROProjector {
    
public:
    
    /*
     Project the given world coordinates into screen coordinates.
     */
    static bool project(const VROVector3f pos, const float *mvp, const int *viewport, VROVector3f *result);
    
    /*
     Unproject the given screen coordinates into world coordinates. The given screen coordinate vector must
     contain a Z element in the range [0,1], where 0 is the near clipping plane and 1 the far clipping plane.
     */
    static bool unproject(const VROVector3f screen, const float *mvp, const int *viewport, VROVector3f *result);
    
};

#endif /* VROProjector_h */
