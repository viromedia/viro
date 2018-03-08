//
//  VRTPolygon.mm
//  ViroReact
//
//  Copyright © 2018 Viro Media. All rights reserved.
//

#import <ViroKit/ViroKit.h>
#import <React/RCTLog.h>
#import "VRTPolygon.h"
#import "VRTUtils.h"

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
        RCTLogError(@"VRTPolygon should contain at least 1 vertex.");
    }

    _vertices = vertices;
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

    _surface = VROPolygon::createPolygon(nativePoints,
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
