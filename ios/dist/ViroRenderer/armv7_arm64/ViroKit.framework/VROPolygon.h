//
//  VROPolygon.h
//  ViroRenderer
//
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROPolygon_h
#define VROPolygon_h

#include "VROGeometry.h"
#include "VROShapeUtils.h"
#include "VROByteBuffer.h"
#include <memory>

namespace p2t {
    class Point;
}

/*
 A geometric representation of a flat Polygon, constructed with N vertices that describes its shape.
 */
class VROPolygon : public VROGeometry {
public:
    static std::shared_ptr<VROPolygon> createPolygon(std::vector<VROVector3f> path,
                                                     float u0 = 0, float v0 = 0, float u1 = 1, float v1 = 1);
    static std::shared_ptr<VROPolygon> createPolygon(std::vector<VROVector3f> path,
                                                     std::vector<std::vector<VROVector3f>> holes,
                                                     float u0 = 0, float v0 = 0, float u1 = 1, float v1 = 1);
    virtual ~VROPolygon();

protected:
    
    VROPolygon(std::vector<VROVector3f> path, std::vector<std::vector<VROVector3f>> holes,
               float u0, float v0, float u1, float v1);

private:
    
    /*
     A vector of 3D coordinates representing the perimeter of this polygon shape, in local
     model space.
     */
    std::vector<VROVector3f> _path;
    std::vector<std::vector<VROVector3f>> _holes;

    /*
     The min and max horizontal and vertical values of this polygon shape, in local
     model space.
     */
    float _minX, _maxX, _minY, _maxY;

    /*
     Texture coordinates with which to use as UV edges for bounded materials.
     */
    float _u0, _v0, _u1, _v1;

    /*
     Rebuilds the sources and elements for this geometric shape with the latest set of
     boundary vertices.
     */
    void updateSurface();
    void buildGeometry(std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                       std::vector<std::shared_ptr<VROGeometryElement>> &elements);
    std::shared_ptr<VROGeometryElement> buildElement(size_t numCorners);
    void writePolygonCorner(p2t::Point *position, VROByteBuffer &buffer);
};

#endif /* VROPolygon_h */
