//
//  VROPolygon.h
//  ViroRenderer
//
//  Copyright © 2018 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

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

    /*
     Poly2Tri only processes paths with non repeating points, otherwise it would crash
     the application. Thus, removeDuplicateVertices() is used to sanitize and remove
     duplicated vertices for a given path.
     */
    void removeDuplicateVertices(std::vector<VROVector3f> &path);
};

#endif /* VROPolygon_h */
