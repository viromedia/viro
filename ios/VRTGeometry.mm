//
//  VRTGeometry.m
//  ViroReact
//
//  Created by Raj Advani on 9/10/18.
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
//

#import "VRTGeometry.h"
#import <React/RCTLog.h>

@implementation VRTGeometry {
    std::shared_ptr<VROGeometry> _geometry;
    BOOL _geometryNeedsUpdate;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _geometry = std::make_shared<VROGeometry>();
        _geometryNeedsUpdate = YES;
        [self node]->setGeometry(_geometry);
    }
    
    return self;
}

- (void)setVertices:(NSArray<NSArray<NSNumber *> *> *)vertices {
    if ([vertices count] < 1) {
        RCTLogError(@"VRTGeometry should contain at least 1 vertex");
    }
    _vertices = vertices;
    _geometryNeedsUpdate = YES;
}

- (void)setNormals:(NSArray<NSArray<NSNumber *> *> *)normals {
    _normals = normals;
    _geometryNeedsUpdate = YES;
}

- (void)setTexcoords:(NSArray<NSArray<NSNumber *> *> *)texcoords {
    _texcoords = texcoords;
    _geometryNeedsUpdate = YES;
}

- (void)setTriangleIndices:(NSArray<NSArray<NSNumber *> *> *)triangles {
    _triangleIndices = triangles;
    _geometryNeedsUpdate = YES;
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    if (_geometryNeedsUpdate) {
        [self updateGeometry];
        _geometryNeedsUpdate = NO;
    }
}

- (std::vector<float>)convertFloatArray:(NSArray<NSArray<NSNumber *> *> *)array numCoordinatesPerPoint:(int)coordsPerPoint type:(NSString *)type {
    std::vector<float> nativePoints;
    for (int i = 0; i < [array count]; i++) {
        NSArray *pointArray = [array objectAtIndex:i];
        if (!pointArray || [pointArray count] < coordsPerPoint) {
            RCTLogError(@"ViroGeometry %@ should have at least %d coordinates", type, coordsPerPoint);
            continue;
        } else if ([pointArray count] != coordsPerPoint) {
            RCTLogWarn(@"ViroGeometry %@ requires %d coordinates per point, but was given a point with %d coordinates",
                       type, coordsPerPoint, (int) [pointArray count]);
        }
        
        if (coordsPerPoint > 0) {
            nativePoints.push_back([[pointArray objectAtIndex:0] floatValue]);
        }
        if (coordsPerPoint > 1) {
            nativePoints.push_back([[pointArray objectAtIndex:1] floatValue]);
        }
        if (coordsPerPoint > 2) {
            nativePoints.push_back([[pointArray objectAtIndex:2] floatValue]);
        }
    }
    return nativePoints;
}

- (std::vector<int>)convertIntArray:(NSArray<NSNumber *> *)array {
    std::vector<int> nativePoints;
    for (int i = 0; i < [array count]; i++) {
        nativePoints.push_back([[array objectAtIndex:i] intValue]);
    }
    return nativePoints;
}

- (void)updateGeometry {
    if (_vertices == nullptr || [_vertices count] <= 0) {
        RCTLogError(@"Error: Attempted to construct a geometry with no vertices!");
        return;
    }
    
    std::vector<float> vertices = [self convertFloatArray:_vertices numCoordinatesPerPoint:3 type:@"vertices"];
    int numVertices = (int) vertices.size() / 3;
    std::shared_ptr<VROData> vertexData = std::make_shared<VROData>((void *) vertices.data(), vertices.size() * sizeof(float));
    
    std::vector<std::shared_ptr<VROGeometrySource>> sources;
    sources.push_back(std::make_shared<VROGeometrySource>(vertexData,
                                                          VROGeometrySourceSemantic::Vertex,
                                                          numVertices,
                                                          true, 3,
                                                          sizeof(float),
                                                          0,
                                                          sizeof(float) * 3));
    
    std::vector<float> normals = [self convertFloatArray:_normals numCoordinatesPerPoint:3 type:@"normals"];
    int numNormals = (int) normals.size() / 3;
    std::shared_ptr<VROData> normalData = std::make_shared<VROData>((void *) normals.data(), normals.size() * sizeof(float));
    
    sources.push_back(std::make_shared<VROGeometrySource>(normalData,
                                                          VROGeometrySourceSemantic::Normal,
                                                          numNormals,
                                                          true, 3,
                                                          sizeof(float),
                                                          0,
                                                          sizeof(float) * 3));
    
    std::vector<float> texcoords = [self convertFloatArray:_texcoords numCoordinatesPerPoint:2 type:@"texcoords"];
    int numTexcoords = (int) texcoords.size() / 2;
    std::shared_ptr<VROData> texcoordData = std::make_shared<VROData>((void *) texcoords.data(), texcoords.size() * sizeof(float));
    
    sources.push_back(std::make_shared<VROGeometrySource>(texcoordData,
                                                          VROGeometrySourceSemantic::Texcoord,
                                                          numTexcoords,
                                                          true, 2,
                                                          sizeof(float),
                                                          0,
                                                          sizeof(float) * 2));
    _geometry->setSources(sources);
    
    std::vector<std::shared_ptr<VROGeometryElement>> elements;
    for (int i = 0; i < [_triangleIndices count]; i++) {
        NSArray<NSNumber *> *submesh = [_triangleIndices objectAtIndex:i];
        
        std::vector<int> triangles = [self convertIntArray:submesh];
        int numIndices = (int) triangles.size();
        int indexSize = sizeof(int);
        std::shared_ptr<VROData> indicesData = std::make_shared<VROData>((void *) triangles.data(), numIndices * indexSize);
        
        elements.push_back(std::make_shared<VROGeometryElement>(indicesData,
                                                                VROGeometryPrimitiveType::Triangle,
                                                                VROGeometryUtilGetPrimitiveCount(numIndices, VROGeometryPrimitiveType::Triangle),
                                                                indexSize));
    }
    _geometry->setElements(elements);
    [self applyMaterials];
}

@end
