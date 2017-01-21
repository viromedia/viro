//
//  VRTCamera.m
//  React
//
//  Created by Raj Advani on 10/5/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTCamera.h"
#import <ViroKit/ViroKit.h>
#import "VRTMaterialManager.h"

static NSArray *const kDefaultCameraPosition  = @[@0, @0, @0];

@implementation VRTCamera

@synthesize position = _position;

- (instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super initWithBridge:bridge];
  if (self) {
    _position = kDefaultCameraPosition;
  }
  
  return self;
}

- (VROCameraRotationType)rotationType {
  return VROCameraRotationType::Standard;
}

@end
