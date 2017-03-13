//
//  VROSphere.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/20/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROSphere_h
#define VROSphere_h

#include "VROGeometry.h"

class VROSphere : public VROGeometry {
    
public:
    
    static std::shared_ptr<VROSphere> createSphere(float radius, int widthSegments, int heightSegments,
                                                   bool facesOutward);
    virtual ~VROSphere() {}
    
private:
    
    VROSphere(std::vector<std::shared_ptr<VROGeometrySource>> sources,
              std::vector<std::shared_ptr<VROGeometryElement>> elements) :
        VROGeometry(sources, elements)
    {}
    
};

#endif /* VROSphere_h */
