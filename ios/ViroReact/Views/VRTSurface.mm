//
//  VRTSurface.mm
//  ViroReact
//
//  Created by Andy Chu on 1/11/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//
#import <ViroKit/ViroKit.h>
#import <React/RCTLog.h>
#import "VRTSurface.h"


static float const kDefaultWidth = 1;
static float const kDefaultHeight = 1;

@implementation VRTSurface {
    std::shared_ptr<VROSurface> _surface;
    BOOL _surfaceNeedsUpdate;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _width = kDefaultWidth;
        _height = kDefaultHeight;
        _surface = VROSurface::createSurface(_width, _height);
        [self node]->setGeometry(_surface);
        _surfaceNeedsUpdate = NO;
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

- (void)updateGeometry {
    _surface = VROSurface::createSurface([self width], [self height]);
    [self node]->setGeometry(_surface);
    [self applyMaterials];
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    if(_surfaceNeedsUpdate) {
        [self updateGeometry];
        _surfaceNeedsUpdate = NO;
    }
}


@end
