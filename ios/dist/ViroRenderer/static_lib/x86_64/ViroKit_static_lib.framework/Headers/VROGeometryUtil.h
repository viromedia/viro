//
//  VROGeometryUtil.h
//  ViroRenderer
//
//  Created by Raj Advani on 3/2/16.
//  Copyright © 2016 Viro Media. All rights reserved.
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

#ifndef VROGeometryUtil_h
#define VROGeometryUtil_h

#include <stdio.h>
#include <memory>
#include <vector>

class VROData;
class VROVector3f;
class VROGeometryElement;
class VROGeometrySource;
class VRONode;
enum class VROGeometrySourceSemantic;
enum class VROGeometryPrimitiveType;

/*
 Return one VRONode per VROGeometryElement. The position of the node will be set
 to the center of each element's vertices.
 */
std::vector<std::shared_ptr<VRONode>> VROGeometryUtilSplitNodeByGeometryElements(std::shared_ptr<VRONode> node);

/*
 Find the vertices used by the geometry element in the given source and center them.
 Return the newly allocated, centered vertex data. Store the center in the given out
 variable.
 */
std::shared_ptr<VROData> VROGeometryUtilExtractAndCenter(std::shared_ptr<VROGeometryElement> element,
                                                         std::shared_ptr<VROGeometrySource> geometrySource,
                                                         VROVector3f *outCenter);

/*
 Get how many indices are required to render the given number of primitives of the
 given type, and vice-versa.
 */
int VROGeometryUtilGetIndicesCount(int primitiveCount, VROGeometryPrimitiveType primitiveType);
int VROGeometryUtilGetPrimitiveCount(int indicesCount, VROGeometryPrimitiveType primitiveType);


/*
 Parse the attribute index for the given semantic.
 */
int VROGeometryUtilParseAttributeIndex(VROGeometrySourceSemantic semantic);

#endif /* VROGeometryUtil_h */
