//
//  VROGeometrySource.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/17/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROGeometrySource_h
#define VROGeometrySource_h

#include <stdio.h>
#include <memory>
#include <functional>
#include "VROData.h"
#include "VROBoundingBox.h"

class VROGeometrySourceSubstrate;

enum class VROGeometrySourceSemantic {
    Vertex,
    Normal,
    Color,
    Texcoord,
    VertexCrease,
    EdgeCrease,
    BoneWeights,
    BoneIndices,
    Invalid
};

/*
 Specifies per-vertex data for the set of vertices forming the surface of a three-dimensional 
 object, or geometry. Used together with VROGeometryElement objects to define custom VROGeometry objects
 or to inspect the data that composes an existing geometry.
 */
class VROGeometrySource {
    
public:
    
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
    
    std::shared_ptr<VROData> getData() const {
        return _data;
    }
    VROGeometrySourceSemantic getSemantic() const {
        return _semantic;
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
    
    /*
     Read through all the vertices in this data source and invoke the provided
     callback for each.
     */
    void processVertices(std::function<void(int index, VROVector3f vertex)> function) const;
    
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
     The interleaved raw vertex data.
     */
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
     The representation of this geometry source in the underlying graphics
     engine.
     */
    VROGeometrySourceSubstrate *_substrate;
    
};

#endif /* VROGeometrySource_h */
