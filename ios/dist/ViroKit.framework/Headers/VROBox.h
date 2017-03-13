//
//  VROBox.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/7/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROBox_h
#define VROBox_h

#include "VROGeometry.h"
#include "VROShapeUtils.h"
#include <memory>

class VROBox : public VROGeometry {
    
public:
    
    static std::shared_ptr<VROBox> createBox(float width, float height, float length);
    virtual ~VROBox();
    
private:
    
    VROBox(std::vector<std::shared_ptr<VROGeometrySource>> sources,
           std::vector<std::shared_ptr<VROGeometryElement>> elements) :
        VROGeometry(sources, elements)
    {}
    
    static void buildBox(VROShapeVertexLayout *vertexLayout, float width, float height,
                         float length);

};

#endif /* VROBox_h */
