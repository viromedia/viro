//
//  VROShapeUtils.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/3/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROShapeUtils_h
#define VROShapeUtils_h

#include <stdio.h>
#include <vector>
#include <memory>

class VROData;
class VROVector3f;
class VROGeometrySource;

typedef struct {
    // Position
    float x;
    float y;
    float z;
    
    // Tex-coords
    float u;
    float v;
    
    // Normal
    float nx;
    float ny;
    float nz;
    
    // Tangent
    float tx;
    float ty;
    float tz;
    float tw;
} VROShapeVertexLayout;

/*
 Compute the tangents for the given shape and store them.
 */
void VROShapeUtilComputeTangents(VROShapeVertexLayout *vertexLayout, size_t verticesLength,
                                 int *indices, size_t indicesLength);

/*
 Three-part function for computing tangents across different index arrays, but the same
 vertex array.
 */
VROVector3f *VROShapeUtilStartTangents(VROShapeVertexLayout *vertexLayout, size_t verticesLength);
void VROShapeUtilComputeTangentsForIndices(VROShapeVertexLayout *vertexLayout, size_t verticesLength,
                                           int *indices, size_t indicesLength, VROVector3f *tan1);
void VROShapeUtilEndTangents(VROShapeVertexLayout *vertexLayout, size_t verticesLength, VROVector3f *tan1);

/*
 Build the geometry sources for rendering the given vertex data.
 */
std::vector<std::shared_ptr<VROGeometrySource>> VROShapeUtilBuildGeometrySources(std::shared_ptr<VROData> vertexData,
                                                                                 size_t numVertices);

#endif /* VROShapeUtils_h */
