//
//  VROPolyline.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/12/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#include <memory>
#include <vector>
#include "VROGeometry.h"
#include "VROAnimatable.h"

class VROVector3f;
class VROByteBuffer;
class VROLineSegment;
class VROShaderModifier;

class VROPolyline : public VROGeometry {
    
public:
    
    static std::shared_ptr<VROPolyline> createPolyline(std::vector<VROVector3f> &path, float thickness);
    virtual ~VROPolyline() {}
    
    /*
     Set the thickness. Animatable.
     */
    void setThickness(float thickness);
    float getThickness() const {
        return _thickness;
    }

    virtual void setMaterials(std::vector<std::shared_ptr<VROMaterial>> materials);

private:
    
    static void buildGeometry(std::vector<VROVector3f> &path,
                              std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                              std::vector<std::shared_ptr<VROGeometryElement>> &elements);
    
    VROPolyline(std::vector<std::shared_ptr<VROGeometrySource>> sources,
                std::vector<std::shared_ptr<VROGeometryElement>> elements,
                float thickness) :
        VROGeometry(sources, elements),
        _thickness(thickness)
    {}
    
    static size_t encodeLine(const std::vector<VROVector3f> path, VROByteBuffer &outBuffer);
    static size_t encodeQuad(VROLineSegment segment, bool beginDegenerate, bool endDegenerate, VROByteBuffer &buffer);
    static size_t encodeCircularEndcap(VROVector3f center, bool beginDegenerate, bool endDegenerate, VROByteBuffer &buffer);

    static void writeCorner(VROVector3f position, VROVector3f normal, VROByteBuffer &buffer);
    static std::shared_ptr<VROShaderModifier> createPolylineShaderModifier();
    
    float _thickness;
    
};
