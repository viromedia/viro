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
    virtual ~VROSurface();
    
protected:
    
    VROSurface(std::vector<std::shared_ptr<VROGeometrySource>> sources,
               std::vector<std::shared_ptr<VROGeometryElement>> elements) :
        VROGeometry(sources, elements)
    {}
    
    static void buildGeometry(float width, float height,
                              float u0, float v0, float u1, float v1,
                              std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                              std::vector<std::shared_ptr<VROGeometryElement>> &elements);
    static void buildSurface(VROShapeVertexLayout *vertexLayout, float width, float height,
                             float u0, float v0, float u1, float v1);
    
};

#endif /* VROSurface_h */
