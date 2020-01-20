//
//  VROImage.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/3/16.
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
