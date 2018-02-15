//
//  VROTorusKnot.hpp
//  ViroRenderer
//
//  Created by Raj Advani on 1/11/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROTorusKnot_h
#define VROTorusKnot_h

#include "VROGeometry.h"
#include <vector>
#include <memory>

class VROTorusKnot : public VROGeometry {
  
public:
    
    static std::shared_ptr<VROTorusKnot> createTorusKnot(float p, float q, float tubeRadius,
                                                         int segments, int slices);
    virtual ~VROTorusKnot();
    
private:
    
    VROTorusKnot(std::vector<std::shared_ptr<VROGeometrySource>> sources,
                 std::vector<std::shared_ptr<VROGeometryElement>> elements) :
        VROGeometry(sources, elements)
    {}
    
};


#endif /* VROTorusKnot_h */
