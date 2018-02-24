//
//  VRTCameraManager.mm
//  React
//
//  Created by Raj Advani on 10/5/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTCameraManager.h"
#import "VRTCamera.h"

@implementation VRTCameraManager

RCT_EXPORT_MODULE()
RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotation, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(animation, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(fieldOfView, float)
RCT_EXPORT_VIEW_PROPERTY(onAnimationStartViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onAnimationFinishViro, RCTDirectEventBlock)

- (VRTView *)view {
    return [[VRTCamera alloc] initWithBridge:self.bridge];
}

@end
