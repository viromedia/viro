//
//  VRTSphere.mm
//  React
//
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
//

#import <ViroKit/ViroKit.h>
#import <React/RCTLog.h>
#import "VRTSphere.h"
#import "VRTMaterialManager.h"


static NSUInteger const kDefaultWidthSegment = 20;
static NSUInteger const kDefaultHeightSegment = 20;
static float const kDefaultRadius = 1;
static BOOL const kDefaultFacesOutward = YES;

@implementation VRTSphere {
    std::shared_ptr<VROSphere> _sphere;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _widthSegmentCount = kDefaultWidthSegment;
        _heightSegmentCount = kDefaultHeightSegment;
        _facesOutward = kDefaultFacesOutward;
        _radius = kDefaultRadius;
        [self updateGeometry];
    }
    
    return self;
}

- (void)setRadius:(float)radius {
    if(radius <= 0) {
        RCTLogError(@"Radius must be greater than 0");
    }
    _radius = radius;
    [self updateGeometry];
}

- (void)setWidthSegmentCount:(NSUInteger)widthSegmentCount {
    if (widthSegmentCount <= 1) {
        RCTLogError(@"heightSegment must be greater than 1");
    }
    _widthSegmentCount = widthSegmentCount;
    [self updateGeometry];
}

- (void)setHeightSegmentCount:(NSUInteger)heightSegmentCount {
    if (heightSegmentCount <= 1) {
        RCTLogError(@"heightSegment must be greater than 1");
    }
    _heightSegmentCount = heightSegmentCount;
    [self updateGeometry];
}

- (void)setFacesOutward:(BOOL)facesOutward {
    _facesOutward = facesOutward;
    [self updateGeometry];
}

- (void)updateGeometry {
    _sphere = VROSphere::createSphere(_radius, _widthSegmentCount, _heightSegmentCount, _facesOutward);
    [self node]->setGeometry(_sphere);
    [self applyMaterials];
}

@end
