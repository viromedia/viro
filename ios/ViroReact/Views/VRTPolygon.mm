//
//  VRTPolygon.mm
//  ViroReact
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
//

#import <ViroKit/ViroKit.h>
#import <React/RCTLog.h>
#import "VRTPolygon.h"
#import "VRTUtils.h"
#import <React/RCTConvert.h>

@implementation RCTConvert (NSArrayNSArrayNSArray)

+ (NSArray<NSArray<NSArray<NSNumber *> *> *> *)NSArrayArrayArray:(id)json {
    if (!json) {
        RCTLogError(@"Error setting array of holes type, received nil json: %@", json);
        return nil;
    }
    if (![json isKindOfClass:[NSArray class]]) {
        RCTLogError(@"Error setting array of holes type. Array required, recieved: %@", json);
        return nil;
    }
    return (NSArray *)json;
}

@end

@implementation VRTPolygon {
    std::shared_ptr<VROPolygon> _surface;
    BOOL _surfaceNeedsUpdate;
    float _uvCoordinateArr[4];
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _surface = nullptr;
        _arShadowReceiver = NO;
        [self node]->setGeometry(_surface);
        _surfaceNeedsUpdate = NO;
        _uvCoordinateArr[0] = 0; _uvCoordinateArr[1] = 0; _uvCoordinateArr[2] = 1; _uvCoordinateArr[3] = 1;
    }
    return self;
}

- (void)setVertices:(NSArray<NSArray<NSNumber *> *> *)vertices {
    if ([vertices count] < 1) {
        RCTLogError(@"VRTPolygon should contain at least 1 vertex");
    }
    _vertices = vertices;
    _surfaceNeedsUpdate = YES;
}

- (void)setHoles:(NSArray<NSArray<NSArray<NSNumber *> *> *> *)holes {
    _holes = holes;
    _surfaceNeedsUpdate = YES;
}

- (void)setUvCoordinates:(NSArray<NSNumber *> *)uvCoordinates {
    if (!uvCoordinates) {
        _uvCoordinateArr[0] = 0;
        _uvCoordinateArr[1] = 0;
        _uvCoordinateArr[2] = 1;
        _uvCoordinateArr[3] = 1;
    } else if ([uvCoordinates count] != 4) {
        RCTLogError(@"[ViroSurface] Expected 4 numbers, only got %lu", [uvCoordinates count]);
    } else {
        populateFloatArrayFromNSArray(uvCoordinates, _uvCoordinateArr, 4);
    }
    _surfaceNeedsUpdate = YES;
}

- (void)setARShadowReceiver:(BOOL)arShadowReceiver {
    _arShadowReceiver = arShadowReceiver;
    [self applyMaterials];
}

- (void)updateGeometry {
    // Check for valid geometry
    if (_vertices == nullptr || [_vertices count] <=0){
        RCTLogError(@"Error: Attempted to construct a Polygon with no vertices!");
        return;
    }

    // Parse out vertex data needed for VROPolygon geometry.
    std::vector<VROVector3f> nativePoints;
    for (int i = 0; i < [_vertices count]; i++) {
        NSArray *pointArray = [_vertices objectAtIndex:i];
        if (!pointArray || [pointArray count] < 2) {
            RCTLogError(@"ViroPolygon points should have at least 2 coordinates.");
            continue;
        } else if ([pointArray count] > 2) {
            RCTLogWarn(@"[ViroPolygon] Polygon only supports xy coordinates! But a set of 3 points had been provided!");
        }
        VROVector3f nativePoint = VROVector3f([[pointArray objectAtIndex:0] floatValue],
                                              [[pointArray objectAtIndex:1] floatValue],
                                              0);
        nativePoints.push_back(nativePoint);
    }
    
    std::vector<std::vector<VROVector3f>> holes;
    for (int h = 0; h < [_holes count]; h++) {
        std::vector<VROVector3f> hole;
        NSArray *holeArray = [_holes objectAtIndex:h];
        
        for (int i = 0; i < [holeArray count]; i++) {
            NSArray *pointArray = [holeArray objectAtIndex:i];
            if (!pointArray || [pointArray count] < 2) {
                RCTLogError(@"ViroPolygon holes should have at least 2 coordinates");
                continue;
            } else if ([pointArray count] > 2) {
                RCTLogWarn(@"[ViroPolygon] Polygon only supports xy coordinates! But a set of 3 points had been provided for a hole!");
            }
            VROVector3f nativePoint = VROVector3f([[pointArray objectAtIndex:0] floatValue],
                                                  [[pointArray objectAtIndex:1] floatValue],
                                                  0);
            hole.push_back(nativePoint);
        }
        holes.push_back(hole);
    }

    _surface = VROPolygon::createPolygon(nativePoints, holes,
                                         _uvCoordinateArr[0],
                                         _uvCoordinateArr[1],
                                         _uvCoordinateArr[2],
                                         _uvCoordinateArr[3]);
    [self node]->setGeometry(_surface);
    [self applyMaterials];
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    if(_surfaceNeedsUpdate) {
        [self updateGeometry];
        _surfaceNeedsUpdate = NO;
    }
}

- (void)applyMaterials {
    [super applyMaterials];
    if (_arShadowReceiver) {
        VROARShadow::apply(_surface->getMaterials().front());
    }
}

@end
