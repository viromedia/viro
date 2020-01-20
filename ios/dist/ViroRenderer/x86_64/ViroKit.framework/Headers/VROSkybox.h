//
//  VROSkybox.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/11/16.
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

#ifndef VROSkybox_h
#define VROSkybox_h

#include <memory>
#include <vector>
#include "VROGeometry.h"

class VROTexture;
class VROVector4f;

class VROSkybox : public VROGeometry {
    
public:
    
    static std::shared_ptr<VROSkybox> createSkybox(std::shared_ptr<VROTexture> textureCube);
    static std::shared_ptr<VROSkybox> createSkybox(VROVector4f color);
    virtual ~VROSkybox();
    
private:
    
    VROSkybox(std::vector<std::shared_ptr<VROGeometrySource>> sources,
              std::vector<std::shared_ptr<VROGeometryElement>> elements) :
        VROGeometry(sources, elements)
    {}
    
    static std::shared_ptr<VROSkybox> buildSkyboxGeometry();
    
};

#endif /* VROSkybox_h */
