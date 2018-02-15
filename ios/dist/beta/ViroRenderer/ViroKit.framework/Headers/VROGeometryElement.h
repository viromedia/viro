//
//  VROGeometryElement.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/17/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROGeometryElement_h
#define VROGeometryElement_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <functional>
#include "VROData.h"
#include "VROTriangle.h"

class VROGeometrySource;

enum class VROGeometryPrimitiveType {
    Triangle,
    TriangleStrip,
    Line,
    Point
};

/*
 Describes how vertices are connected to form the surface of a three-dimensional object, or geometry.
 Used in conjunction with VROGeometrySource.
 */
class VROGeometryElement {
    
public:
    
    VROGeometryElement(std::shared_ptr<VROData> data,
                       VROGeometryPrimitiveType primitiveType,
                       int primitiveCount,
                       int bytesPerIndex) :
        _primitiveType(primitiveType),
        _primitiveCount(primitiveCount),
        _data(data),
        _bytesPerIndex(bytesPerIndex)
    {}
    
    std::shared_ptr<VROData> getData() const {
        return _data;
    }
    VROGeometryPrimitiveType getPrimitiveType() const {
        return _primitiveType;
    }
    int getPrimitiveCount() const {
        return _primitiveCount;
    }
    int getBytesPerIndex() const {
        return _bytesPerIndex;
    }
    
    /*
     Read through the indices in this element, read the corresponding vertices
     from the given geometry source, and invoke the provided function once per
     triangle.
     */
    void processTriangles(std::function<void(int index, VROTriangle triangle)> function,
                          std::shared_ptr<VROGeometrySource> geometrySource) const;
    
    /*
     Read the indexes in this element, one by one.
     */
    void processIndices(std::function<void(int index, int indexRead)> function) const;
    
private:
    
    /*
     The type of the primitives we should create from the associated geometry
     source using the indices in this element.
     */
    VROGeometryPrimitiveType _primitiveType;
    
    /*
     The number of triangles, triangle strips, etc.
     */
    const int _primitiveCount;
    
    /*
     The index data, and the size of each index.
     */
    std::shared_ptr<VROData> _data;
    const int _bytesPerIndex;
    
};

#endif /* VROGeometryElement_h */
