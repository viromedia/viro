//
//  VRTVideoSurfaceManager.m
//  React
//
//  Created by Vik Advani on 3/24/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <React/RCTUIManager.h>
#import "VRTVideoSurfaceManager.h"
#import "VRTVideoSurface.h"

@implementation VRTVideoSurfaceManager
RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(source, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotation, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scale, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotationPivot, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scalePivot, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(visible, BOOL)
RCT_EXPORT_VIEW_PROPERTY(onHoverViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onClickViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onFuseViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(timeToFuse, float)
RCT_EXPORT_VIEW_PROPERTY(canHover, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canClick, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canFuse, BOOL)
RCT_EXPORT_VIEW_PROPERTY(transformBehaviors, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(opacity, float)
RCT_EXPORT_VIEW_PROPERTY(paused, BOOL)
RCT_EXPORT_VIEW_PROPERTY(loop, BOOL)
RCT_EXPORT_VIEW_PROPERTY(muted, BOOL)
RCT_EXPORT_VIEW_PROPERTY(volume, float)
RCT_EXPORT_VIEW_PROPERTY(width, float)
RCT_EXPORT_VIEW_PROPERTY(height, float)
RCT_EXPORT_VIEW_PROPERTY(onBufferStartViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onBufferEndViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onFinishViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onErrorViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onUpdateTimeViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(stereoMode, NSString)

- (VRTVideoSurface *)view
{
  return [[VRTVideoSurface alloc] initWithBridge:self.bridge];
}

RCT_EXPORT_METHOD(seekToTime:(nonnull NSNumber *)reactTag time:(NSInteger)time)
{
  [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
    VRTView *view = (VRTView *)viewRegistry[reactTag];
    if (![view isKindOfClass:[VRTVideoSurface class]]) {
      RCTLogError(@"Invalid view returned from registry, expecting VRTVideoSurface, got: %@", view);
    } else {
      VRTVideoSurface *component = (VRTVideoSurface *)view;
      [component seekToTime:time];
    }
  }];
}

@end
