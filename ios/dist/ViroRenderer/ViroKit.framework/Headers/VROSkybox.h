//
//  VROSkybox.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/11/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

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
