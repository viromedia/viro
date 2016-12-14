//
//  VROSize.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/15/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROSize_h
#define VROSize_h

class VROSize {
    
public:
    
    float width, height;
    
    VROSize() :
        width(0),
        height(0)
    {}
    
    VROSize(float width, float height) :
        width(width),
        height(height)
    {}
    
};

#endif /* VROSize_h */
