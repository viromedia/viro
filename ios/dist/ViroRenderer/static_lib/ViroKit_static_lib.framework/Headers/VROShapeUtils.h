//
//  VROShapeUtils.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/3/15.
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
