//
//  VRTQuad.mm
//  ViroReact
//
//  Created by Andy Chu on 1/11/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//
#import <ViroKit/ViroKit.h>
#import <React/RCTLog.h>
#import "VRTQuad.h"
#import "VRTUtils.h"


static float const kDefaultWidth = 1;
static float const kDefaultHeight = 1;

@implementation VRTQuad {
    std::shared_ptr<VROSurface> _surface;
    BOOL _surfaceNeedsUpdate;
    float _uvCoordinateArr[4];
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _width = kDefaultWidth;
        _height = kDefaultHeight;
        _surface = VROSurface::createSurface(_width, _height);
        _arShadowReceiver = NO;
        [self node]->setGeometry(_surface);
        _surfaceNeedsUpdate = NO;
        _uvCoordinateArr[0] = 0; _uvCoordinateArr[1] = 0; _uvCoordinateArr[2] = 1; _uvCoordinateArr[3] = 1;
    }
    return self;
}

- (void)setWidth:(float)width {
    if (width < 0) {
        RCTLogError(@"Surface width must be >= 0");
    }
    _width = width;
    _surfaceNeedsUpdate = YES;
}

- (void)setHeight:(float)height {
    if (height < 0) {
        RCTLogError(@"Surface height must be >= 0");
    }
    _height = height;
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

- (void)setArShadowReceiver:(BOOL)arShadowReceiver {
    _arShadowReceiver = arShadowReceiver;
    [self applyMaterials];
}

- (void)updateGeometry {
    _surface = VROSurface::createSurface([self width], [self height], _uvCoordinateArr[0], _uvCoordinateArr[1], _uvCoordinateArr[2], _uvCoordinateArr[3]);
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
