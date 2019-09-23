//
//  VRTQuad.mm
//  ViroReact
//
//  Created by Andy Chu on 1/11/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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
