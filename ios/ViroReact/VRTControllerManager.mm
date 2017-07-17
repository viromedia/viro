//
//  VRTControllerManager.mm
//  React
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import "VRTControllerManager.h"
#import "VRTController.h"

@implementation VRTControllerManager

RCT_EXPORT_MODULE()
RCT_EXPORT_VIEW_PROPERTY(onClickViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(canClick, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canFuse, BOOL)
RCT_EXPORT_VIEW_PROPERTY(onFuseViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(timeToFuse, float)
RCT_EXPORT_VIEW_PROPERTY(reticleVisibility, BOOL)
RCT_EXPORT_VIEW_PROPERTY(controllerVisibility, BOOL)

- (VRTView *)view
{
    return [[VRTController alloc] initWithBridge:self.bridge];
}

@end
