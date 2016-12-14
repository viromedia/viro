//
//  VRTOrbitCamera.m
//  React
//
//  Created by Raj Advani on 10/5/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTOrbitCamera.h"

static NSArray *const kDefaultOrbitFocalPoint = @[@0, @0, @0];

@implementation VRTOrbitCamera

@synthesize focalPoint = _focalPoint;

- (instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super initWithBridge:bridge];
  if (self) {
    _focalPoint = kDefaultOrbitFocalPoint;
  }
  
  return self;
}

- (VROCameraRotationType)rotationType {
  return VROCameraRotationType::Orbit;
}

@end
