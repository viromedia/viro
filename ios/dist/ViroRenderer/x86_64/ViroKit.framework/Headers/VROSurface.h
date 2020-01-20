//
//  VROSurface.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/3/15.
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

#ifndef VROSurface_h
#define VROSurface_h

#include "VROGeometry.h"
#include "VROShapeUtils.h"
#include <memory>

class VROSurface : public VROGeometry {
    
public:
    
    static std::shared_ptr<VROSurface> createSurface(float width, float height,
                                                     float u0 = 0, float v0 = 0, float u1 = 1, float v1 = 1);
    static std::shared_ptr<VROSurface> createSurface(float x, float y, float width, float height,
                                                     float u0, float v0, float u1, float v1);
    virtual ~VROSurface();
    
    /*
     Set physical dimensions of the surface.
     */
    void setX(float x);
    void setY(float y);
    void setWidth(float width);
    void setHeight(float height);
    
    /*
     Set texture coordinates of the surface, and an optional transform.
     */
    void setU0(float u0);
    void setU1(float u1);
    void setV0(float v0);
    void setV1(float v1);
    void setTexcoordTransform(VROMatrix4f transform);
    
    /*
     Directly set the texture coordinates of all four corners.
     */
    void setTextureCoordinates(VROVector3f BL, VROVector3f BR, VROVector3f TL, VROVector3f TR);
    
    float getU0() const { return _u0; }
    float getU1() const { return _u1; }
    float getV0() const { return _v0; }
    float getV1() const { return _v1; }
    
    float getX() const { return _x; }
    float getY() const { return _y; }
    float getWidth() const { return _width; }
    float getHeight() const { return _height; }
    
protected:
    
    VROSurface(float x, float y, float width, float height,
               float u0, float v0, float u1, float v1);
    
private:
    
    void updateSurface();

    void buildGeometry(float x, float y, float width, float height,
                       VROVector3f texBL, VROVector3f texBR, VROVector3f texTL, VROVector3f texTR,
                       std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                       std::vector<std::shared_ptr<VROGeometryElement>> &elements);
    void buildSurface(VROShapeVertexLayout *vertexLayout,
                      float left, float bottom, float right, float top,
                      VROVector3f texBL, VROVector3f texBR, VROVector3f texTL, VROVector3f texTR);
    
    float _x, _y, _width, _height;
    float _u0, _v0, _u1, _v1;
    
    /*
     Transform by which to multiply all texture coordinates.
     */
    VROMatrix4f _texcoordTransform;
    
};

#endif /* VROSurface_h */
