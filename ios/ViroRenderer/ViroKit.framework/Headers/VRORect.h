//
//  VRORect.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/15/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VRORect_h
#define VRORect_h

#include "VROSize.h"
#include "VROVector3f.h"

class VRORect {
    
public:
    
    VROVector3f origin;
    VROSize size;
    
    VRORect()
    {}
    
    VRORect(VROVector3f origin, VROSize size) :
        origin(origin),
        size(size)
    {}
    
};

VRORect VRORectMake(float x, float y, float width, float height);
VRORect VRORectMake(float x, float y, float z, float width, float height);

#endif /* VRORect_h */
