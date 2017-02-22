//
//  VRTSphere.mm
//  React
//
//  Copyright © 2016 Viro Media. All rights reserved.
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

-(instancetype)initWithBridge:(RCTBridge *)bridge {
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

-(void)setRadius:(float)radius {
  if(radius <= 0) {
    RCTLogError(@"Radius must be greater than 0");
  }
  _radius = radius;
  [self updateGeometry];
}

-(void)setWidthSegmentCount:(NSUInteger)widthSegmentCount {
  if (widthSegmentCount <= 1) {
    RCTLogError(@"heightSegment must be greater than 1");
  }
  _widthSegmentCount = widthSegmentCount;
  [self updateGeometry];
}

-(void)setHeightSegmentCount:(NSUInteger)heightSegmentCount {
  if (heightSegmentCount <= 1) {
    RCTLogError(@"heightSegment must be greater than 1");
  }
  _heightSegmentCount = heightSegmentCount;
  [self updateGeometry];
}

-(void)setFacesOutward:(BOOL)facesOutward {
  _facesOutward = facesOutward;
  [self updateGeometry];
}

-(void)updateGeometry {
  _sphere = VROSphere::createSphere(_radius, _widthSegmentCount, _heightSegmentCount, _facesOutward);
  [self node]->setGeometry(_sphere);
  [self applyMaterials];
}

@end
