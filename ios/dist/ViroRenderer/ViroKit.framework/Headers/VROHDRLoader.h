//
//  VROHDRLoader.h
//  ViroKit
//
//  Created by Raj Advani on 1/22/18.
//  Copyright © 2018 Viro Media. All rights reserved.
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

#ifndef VROHDRLoader_h
#define VROHDRLoader_h

#include <stdio.h>
#include <string>
#include <memory>

class VROTexture;
enum class VROTextureInternalFormat;

/*
 Converts HDR images to RGB9_E5 format so that they can be read by OpenGL ES.
 */
class VROHDRLoader {
public:
    
    /*
     Loads the Radiance HDR texture (.hdr) at the given path. The data will
     be internally stored in RGB9_E5 format.
     */
    static std::shared_ptr<VROTexture> loadRadianceHDRTexture(std::string hdrPath);
    
private:
    
    static std::shared_ptr<VROTexture> loadTexture(float *data, int width, int height,
                                                   int componentsPerPixel);
    
};

#endif /* VROHDRLoader_h */
