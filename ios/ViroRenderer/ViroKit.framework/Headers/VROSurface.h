//
//  VROSurface.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/3/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

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
    
    void setX(float x);
    void setY(float y);
    void setWidth(float width);
    void setHeight(float height);
    
protected:
    
    VROSurface(float x, float y, float width, float height,
               float u0, float v0, float u1, float v1);
    
private:
    
    void updateSurface();

    void buildGeometry(float x, float y, float width, float height,
                       float u0, float v0, float u1, float v1,
                       std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                       std::vector<std::shared_ptr<VROGeometryElement>> &elements);
    void buildSurface(VROShapeVertexLayout *vertexLayout,
                      float left, float bottom, float right, float top,
                      float u0, float v0, float u1, float v1);
    
    float _x, _y, _width, _height;
    float _u0, _v0, _u1, _v1;
    
};

#endif /* VROSurface_h */
