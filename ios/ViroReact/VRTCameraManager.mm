//
//  VRTCameraManager.m
//  React
//
//  Created by Raj Advani on 10/5/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTCameraManager.h"
#import "VRTCamera.h"
#import "ViroUIManager.h"

@implementation VRTCameraManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(position, NSArray<NSNumber *>)

- (VRTView *)view {
  return [[VRTCamera alloc] initWithBridge:self.bridge];
}

@end
