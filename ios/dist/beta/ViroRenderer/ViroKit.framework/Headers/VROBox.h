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
