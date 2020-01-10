//
//  VROBox.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/7/15.
//  Copyright © 2015 Viro Media. All rights reserved.
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

#ifndef VROBox_h
#define VROBox_h

#include "VROGeometry.h"
#include "VROShapeUtils.h"
#include <memory>

class VROBox : public VROGeometry {
    
public:
    
    static std::shared_ptr<VROBox> createBox(float width, float height, float length);
    virtual ~VROBox();
    
    void setWidth(float width);
    void setHeight(float height);
    void setLength(float length);
    
    virtual void setMaterials(std::vector<std::shared_ptr<VROMaterial>> materials);
    
private:
    
    float _width, _height, _length;
    VROBox(float width, float height, float length);
    
    void updateBox();
    void buildBoxVAR(VROShapeVertexLayout *vertexLayout);
    
};

#endif /* VROBox_h */
