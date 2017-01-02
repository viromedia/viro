//
//  VRTAnimatedComponentManager.m
//  React
//
//  Created by Vik Advani on 2/19/16.
//  Copyright © 2016 Facebook. All rights reserved.
//

#import "VRTAnimatedComponentManager.h"
#import "VRTAnimatedComponent.h"
#import "ViroUIManager.h"
#import "VRTAnimatedComponentShadowView.h"

@implementation VRTAnimatedComponentManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(animation, NSString);
RCT_EXPORT_VIEW_PROPERTY(loop, BOOL);
RCT_EXPORT_VIEW_PROPERTY(delay, float);
RCT_EXPORT_VIEW_PROPERTY(run, BOOL);
RCT_EXPORT_VIEW_PROPERTY(onFinish, RCTDirectEventBlock);

- (VRTView *)view
{
  return [[VRTAnimatedComponent alloc] initWithBridge:self.bridge];
}

- (VRTShadowView *)shadowView {
  return [VRTAnimatedComponentShadowView new];
}

RCT_EXPORT_METHOD(startAnimation:(nonnull NSNumber *)reactTag)
{
  [self.bridge.viroUIManager addUIBlock:^(__unused ViroUIManager *uiManager, NSDictionary<NSNumber *, VRTView *> *viewRegistry) {
    VRTView *view = viewRegistry[reactTag];
    if (![view isKindOfClass:[VRTAnimatedComponent class]]) {
      RCTLogError(@"Invalid view returned from registry, expecting VRTAnimatedComponent, got: %@", view);
    } else {
      VRTAnimatedComponent *component = (VRTAnimatedComponent *)view;
      [component setRun:YES];
    }
  }];
}

RCT_EXPORT_METHOD(stopAnimation:(nonnull NSNumber *)reactTag)
{
  [self.bridge.viroUIManager addUIBlock:^(__unused ViroUIManager *uiManager, NSDictionary<NSNumber *, VRTView *> *viewRegistry) {
      VRTView *view = viewRegistry[reactTag];
      if (![view isKindOfClass:[VRTAnimatedComponent class]]) {
        RCTLogError(@"Invalid view returned from registry, expecting VRTAnimatedComponent, got: %@", view);
      } else {
        VRTAnimatedComponent *component = (VRTAnimatedComponent *)view;
          [component setRun:NO];
      }
  }];
}

@end
