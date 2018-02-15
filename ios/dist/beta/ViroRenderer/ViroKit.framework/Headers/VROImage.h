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
#include "VROTexture.h"

class VROImage {
    
public:
    
    VROImage() {}
    virtual ~VROImage() {}
    
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    
    /*
     The data returned must be compatible with (convertible to) _format, 
     as per the table in the OpenGL ES spec:
     
     https://www.khronos.org/registry/OpenGL-Refpages/es3.0/html/glTexImage2D.xhtml
     
     E.g., if internal format is RGB565, the data returned should be RGB8 or 
     RGB565. If internal format is RGBA8 or RGBA4, the data returned should be
     RGBA8, etc.
     */
    virtual unsigned char *getData(size_t *length) = 0;
    
    VROTextureFormat getFormat() {
        return _format;
    }
    
protected:
    
    /*
     The format of the data returned by getData.
     */
    VROTextureFormat _format;
    
    
};

#endif /* VROImage_h */
