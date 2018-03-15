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
    
    /*
     Lock and unlock should be invoked before and after getData(). For most
     image implementations this isn't necessary, but it is for others.
     */
    virtual void lock() {}
    virtual void unlock() {}
    
    /*
     Get the format of the source image data.
     */
    VROTextureFormat getFormat() {
        return _format;
    }
    
    /*
     Get the format of the data we converted the source into.
     */
    VROTextureInternalFormat getInternalFormat() {
        return _internalFormat;
    }
    
protected:
    
    /*
     The format of the source image data.
     */
    VROTextureFormat _format;
    
    /*
     The format of the data returned by getData().
     */
    VROTextureInternalFormat _internalFormat;
    
};

#endif /* VROImage_h */
