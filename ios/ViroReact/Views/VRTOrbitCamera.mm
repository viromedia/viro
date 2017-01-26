//
//  VRTOrbitCamera.m
//  React
//
//  Created by Raj Advani on 10/5/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTOrbitCamera.h"
#import "VRTUtils.h"

static NSArray *const kDefaultOrbitFocalPoint = @[@0, @0, @0];

@implementation VRTOrbitCamera

@synthesize focalPoint = _focalPoint;

- (instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super initWithBridge:bridge];
  if (self) {
    self.focalPoint = kDefaultOrbitFocalPoint;
    self.nodeCamera->setRotationType(VROCameraRotationType::Orbit);
  }
  
  return self;
}

- (void)setFocalPoint:(NSArray<NSNumber *> *)focal {
    _focalPoint = [focal copy];
    
    float values[3];
    populateFloatArrayFromNSArray(focal, values, 3);
    self.nodeCamera->setOrbitFocalPoint({values[0], values[1], values[2]});
}

- (VROCameraRotationType)rotationType {
  return VROCameraRotationType::Orbit;
}

@end
