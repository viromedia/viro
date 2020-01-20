//
//  VROGeometrySource.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/17/15.
//  Copyright © 2015 Viro Media. All rights reserved.
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

#ifndef VROGeometrySource_h
#define VROGeometrySource_h

#include <stdio.h>
#include <memory>
#include <functional>
#include "VROData.h"
#include "VROBoundingBox.h"
#include "VROGeometryElement.h"

enum class VROGeometrySourceSemantic {
    Vertex,
    Normal,
    Color,
    Texcoord,
    Tangent,
    VertexCrease,
    EdgeCrease,
    BoneWeights,
    BoneIndices,
    Morph_0,
    Morph_1,
    Morph_2,
    Morph_3,
    Morph_4,
    Morph_5,
    Morph_6,
    Invalid
};

class VROVertexBuffer;

/*
 Specifies per-vertex data for the set of vertices forming the surface of a three-dimensional 
 object, or geometry. Used together with VROGeometryElement objects to define custom VROGeometry objects
 or to inspect the data that composes an existing geometry.
 */
class VROGeometrySource {
    
public:
    
    /*
     Geometry sources can be built either with VBO data or with raw CPU data. Using the former
     allows the same data to be shared across different geometries, while using
     the latter siloes the data to only this geometry.
     */
    VROGeometrySource(std::shared_ptr<VROData> data,
                      VROGeometrySourceSemantic semantic,
                      int vertexCount,
                      bool floatComponents,
                      int componentsPerVertex,
                      int bytesPerComponent,
                      int dataOffset,
                      int dataStride) :
        _data(data),
        _semantic(semantic),
        _vertexCount(vertexCount),
        _floatComponents(floatComponents),
        _componentsPerVertex(componentsPerVertex),
        _bytesPerComponent(bytesPerComponent),
        _dataOffset(dataOffset),
        _dataStride(dataStride)
    {}
    
    VROGeometrySource(std::shared_ptr<VROData> data,
                      std::shared_ptr<VROGeometrySource> templateSource) :
        _data(data),
        _semantic(templateSource->getSemantic()),
        _vertexCount(templateSource->getVertexCount()),
        _floatComponents(templateSource->isFloatComponents()),
        _componentsPerVertex(templateSource->getComponentsPerVertex()),
        _bytesPerComponent(templateSource->getBytesPerComponent()),
        _dataOffset(templateSource->getDataOffset()),
        _dataStride(templateSource->getDataStride())
    {}
    
    VROGeometrySource(std::shared_ptr<VROVertexBuffer> vbo,
                      VROGeometrySourceSemantic semantic,
                      int vertexCount,
                      bool floatComponents,
                      int componentsPerVertex,
                      int bytesPerComponent,
                      int dataOffset,
                      int dataStride) :
        _vbo(vbo),
        _semantic(semantic),
        _vertexCount(vertexCount),
        _floatComponents(floatComponents),
        _componentsPerVertex(componentsPerVertex),
        _bytesPerComponent(bytesPerComponent),
        _dataOffset(dataOffset),
        _dataStride(dataStride)
    {}
    
    VROGeometrySource(std::shared_ptr<VROVertexBuffer> vbo,
                      std::shared_ptr<VROGeometrySource> templateSource) :
        _vbo(vbo),
        _semantic(templateSource->getSemantic()),
        _vertexCount(templateSource->getVertexCount()),
        _floatComponents(templateSource->isFloatComponents()),
        _componentsPerVertex(templateSource->getComponentsPerVertex()),
        _bytesPerComponent(templateSource->getBytesPerComponent()),
        _dataOffset(templateSource->getDataOffset()),
        _dataStride(templateSource->getDataStride())
    {}
    
    std::shared_ptr<VROVertexBuffer> getVertexBuffer() const {
        return _vbo;
    }
    std::shared_ptr<VROData> getData() const;
    
    VROGeometrySourceSemantic getSemantic() const {
        return _semantic;
    }
    void setSemantic(VROGeometrySourceSemantic semantic) {
        _semantic = semantic;
    }
    int getVertexCount() const {
        return _vertexCount;
    }
    int getComponentsPerVertex() const {
        return _componentsPerVertex;
    }
    int getBytesPerComponent() const {
        return _bytesPerComponent;
    }
    int getDataOffset() const {
        return _dataOffset;
    }
    int getDataStride() const {
        return _dataStride;
    }
    bool isFloatComponents() const {
        return _floatComponents;
    }
    void setGeometryElementIndex(int i) {
        _geoElementIndex = i;
    }
    int getGeometryElementIndex() {
        return _geoElementIndex;
    }
    
    /*
     Read through all the vertices in this data source and invoke the provided
     callback for each.
     */
    void processVertices(std::function<void(int index, VROVector4f vertex)> function) const;
    
    /*
     Read through all vertices in this data source and modify them.
     */
    void modifyVertices(std::function<VROVector3f(int index, VROVector3f vertex)> function) const;
    
    /*
     Retrieves the bounding box for the values associated with this
     geometry source. Generally only makes sense to invoke this method
     for VROGeometrySourceSemantic::Vertex.
     */
    VROBoundingBox getBoundingBox() const;
    
private:
    
    /*
     The interleaved raw vertex data. This is either in the form of a VBO, or as CPU-based data.
     Using the former allows the same data to be shared across different geometries, while using
     the latter siloes the data to only this geometry.
     */
    std::shared_ptr<VROVertexBuffer> _vbo;
    std::shared_ptr<VROData> _data;
    
    /*
     The attribute the geometry source is describing for each vertex.
     */
    VROGeometrySourceSemantic _semantic;
    
    /*
     The number of vertices described in the _data array.
     */
    int _vertexCount;
    
    /*
     True if the components are floating point. False if integer.
     */
    bool _floatComponents;
    
    /*
     The number of scalar components per vertex (i.e. x, y, z for position).
     */
    int _componentsPerVertex;
    
    /*
     The number of bytes per scalar component (i.e. 4 for floats).
     */
    int _bytesPerComponent;
    
    /*
     The offset in the data array at which point this source's data begins. Used
     for interleaving a vertex array.
     */
    int _dataOffset;
    
    /*
     The number of bytes from one vertex in the data to the next.
     */
    int _dataStride;

    /*
     The index of the VROGeometryElement that is related to this VROGeometrySource contained within
     the vec of elements in source's VROGeometry. If there is no unique relationship (if the bounded
     geometry elements require all bounded sources), then _geoElementIndex is is -1.
     Note: This is an optional field.
     */
    int _geoElementIndex = -1;
};

#endif /* VROGeometrySource_h */
