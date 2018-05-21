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

/*
 A geometric representation of a flat Polygon, constructed with N vertices that describes its shape.
 */
class VROPolygon : public VROGeometry {
public:
    static std::shared_ptr<VROPolygon> createPolygon(std::vector<VROVector3f> vertices,
                                                     float u0 = 0, float v0 = 0, float u1 = 1, float v1 = 1);
    virtual ~VROPolygon();

protected:
    VROPolygon(std::vector<VROVector3f> boundaries, float u0, float v0, float u1, float v1);

private:
    /*
     A Vector of 3D coordinates representing the boundaries of this polygon shape, in local
     model space.
     */
    std::vector<VROVector3f> _boundaryVertices;

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
    void buildGeometry(std::vector<VROVector3f> boundaryPath,
                       std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                       std::vector<std::shared_ptr<VROGeometryElement>> &elements);

    /*
     Iterates through the vector of _boundaryVertices to construct triangle meshed
     elements to be rendered as indexed vertices representing the polygon shape.
     */
    std::shared_ptr<VROGeometryElement> buildElement(size_t numCorners);
    void writePolygonCorner(VROVector3f position, VROByteBuffer &buffer);
};

#endif /* VROPolygon_h */
