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
    
    static std::shared_ptr<VROSurface> createSurface(float width, float height);
    virtual ~VROSurface();
    
protected:
    
    VROSurface(std::vector<std::shared_ptr<VROGeometrySource>> sources,
               std::vector<std::shared_ptr<VROGeometryElement>> elements) :
        VROGeometry(sources, elements)
    {}
    
    static void buildGeometry(float width, float height,
                              std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                              std::vector<std::shared_ptr<VROGeometryElement>> &elements);
    static void buildSurface(VROShapeVertexLayout *vertexLayout, float width, float height);
    
};

#endif /* VROSurface_h */
