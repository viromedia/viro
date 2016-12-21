//
//  VROImage.hpp
//  ViroRenderer
//
//  Created by Raj Advani on 11/3/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROImage_h
#define VROImage_h

#include <stdio.h>
#include "VRODefines.h"

class VROImage {
    
public:
    
    VROImage() {}
    virtual ~VROImage() {}
    
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    virtual unsigned char *extractRGBA8888(size_t *length) = 0;
    
private:
    
};

#endif /* VROImage_h */