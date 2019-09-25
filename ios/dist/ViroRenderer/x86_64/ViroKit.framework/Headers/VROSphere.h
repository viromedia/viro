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
    VROSphere(float radius, int widthSegments = 20, int heightSegments = 20, bool facesOutward = true);
    virtual ~VROSphere() {}

    void setRadius(float radius);
    void setWidthSegments(int widthSegments);
    void setHeightSegments(int heightSegments);
    void setFacesOutward(bool facesOutward);
    
private:

    float _radius;
    int _widthSegments, _heightSegments;
    bool _facesOutward;
    
    VROSphere(std::vector<std::shared_ptr<VROGeometrySource>> sources,
              std::vector<std::shared_ptr<VROGeometryElement>> elements) :
        VROGeometry(sources, elements)
    {}

    void updateSphere();

};

#endif /* VROSphere_h */
