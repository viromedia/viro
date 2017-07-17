//
//  VRTOrbitCameraManager.m
//  React
//
//  Created by Raj Advani on 10/5/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTOrbitCameraManager.h"
#import "VRTOrbitCamera.h"

@implementation VRTOrbitCameraManager

RCT_EXPORT_MODULE()
RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(focalPoint, NSNumberArray)

- (VRTView *)view {
    return [[VRTOrbitCamera alloc] initWithBridge:self.bridge];
}

@end
