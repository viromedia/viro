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
                       int bytesPerIndex,
                       bool isSigned = true) :
        _primitiveType(primitiveType),
        _primitiveCount(primitiveCount),
        _data(data),
        _bytesPerIndex(bytesPerIndex),
        _signed(isSigned)
    {}

    // The data, primitive type, primitive count, and bytes per index
    // must be manually set when using this constructor
    VROGeometryElement() : _primitiveType(VROGeometryPrimitiveType::Triangle),
                           _primitiveCount(0),
                           _data(nullptr),
                           _bytesPerIndex(sizeof(int))
    {}

    void setData(std::shared_ptr<VROData> data) {
        _data = data;
    }
    std::shared_ptr<VROData> getData() const {
        return _data;
    }

    void setPrimitiveType(VROGeometryPrimitiveType primitiveType) {
        _primitiveType = primitiveType;
    }
    VROGeometryPrimitiveType getPrimitiveType() const {
        return _primitiveType;
    }

    void setPrimitiveCount(int primitiveCount) {
        _primitiveCount = primitiveCount;
    }
    int getPrimitiveCount() const {
        return _primitiveCount;
    }

    void setBytesPerIndex(int bytesPerIndex) {
        _bytesPerIndex = bytesPerIndex;
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
    int _primitiveCount;
    
    /*
     The index data, size of each index, and whether the data is signed.
     */
    std::shared_ptr<VROData> _data;
    int _bytesPerIndex;
    bool _signed;
    
};

#endif /* VROGeometryElement_h */
