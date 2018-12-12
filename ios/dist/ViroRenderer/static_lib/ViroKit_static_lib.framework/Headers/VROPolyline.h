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
    static std::shared_ptr<VROPolyline> createPolyline(std::vector<std::vector<VROVector3f>> &path, float thickness);
    static std::shared_ptr<VROPolyline> createPolyline(std::vector<VROVector3f> &path, float thickness);
    
    VROPolyline();
    virtual ~VROPolyline() {}
    
    /*
     Set the thickness. Animatable.
     */
    void setThickness(float thickness);
    float getThickness() const {
        return _thickness;
    }
    
    /*
     Set the paths of this polyline. Each path is a contiguous line. This will reconstruct
     all paths.
     */
    void setPaths(std::vector<std::vector<VROVector3f>> &paths);
    
    /*
     Append the given point the last path in this polyline. This is more efficient
     than invoking setPaths.
     */
    void appendPoint(VROVector3f point);

    virtual void setMaterials(std::vector<std::shared_ptr<VROMaterial>> materials);

private:
    
    float _thickness;
    
    VROPolyline(std::vector<std::shared_ptr<VROGeometrySource>> sources,
                std::vector<std::shared_ptr<VROGeometryElement>> elements,
                float thickness) :
        VROGeometry(sources, elements),
        _thickness(thickness)
    {}
    
    bool isEmpty() const;
    VROVector3f getLastPoint() const;
    
    static void buildGeometry(std::vector<std::vector<VROVector3f>> &paths,
                              std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                              std::vector<std::shared_ptr<VROGeometryElement>> &elements);
    static std::shared_ptr<VROGeometryElement> buildElement(size_t numCorners);
    static size_t encodeLine(const std::vector<VROVector3f> &path, VROByteBuffer &outBuffer);
    static size_t encodeQuad(VROLineSegment segment, bool beginDegenerate, bool endDegenerate, VROByteBuffer &buffer);
    static size_t encodeCircularEndcap(VROVector3f center, VROVector3f direction,
                                       bool beginDegenerate, bool endDegenerate, VROByteBuffer &buffer);

    static void writeCorner(VROVector3f position, VROVector3f direction, VROByteBuffer &buffer);
    static void writeEndcapCorner(VROVector3f position, VROVector3f direction, float rotation, VROByteBuffer &buffer);
    static void createPolylineShaderModifiers();
    
};
