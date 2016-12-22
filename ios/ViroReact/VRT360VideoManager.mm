//
//  VRO360VideoManager.m
//  React
//
//  Created by Vik Advani on 1/28/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRT360VideoManager.h"
#import "VRT360Video.h"
#import "ViroUIManager.h"

@implementation VRT360VideoManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(source, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(paused, BOOL)
RCT_EXPORT_VIEW_PROPERTY(loop, BOOL)
RCT_EXPORT_VIEW_PROPERTY(muted, BOOL)
RCT_EXPORT_VIEW_PROPERTY(volume, float)
RCT_EXPORT_VIEW_PROPERTY(rotation, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(onFinishViro, RCTDirectEventBlock)

- (VRT360Video *)view
{
  return [[VRT360Video alloc] initWithBridge:self.bridge];
}

RCT_EXPORT_METHOD(seekToTime:(nonnull NSNumber *)reactTag time:(NSInteger)time)
{
  [self.bridge.viroUIManager addUIBlock:^(__unused ViroUIManager *uiManager, NSDictionary<NSNumber *, VRTView *> *viewRegistry) {
    VRTView *view = viewRegistry[reactTag];
    if (![view isKindOfClass:[VRT360Video class]]) {
      RCTLogError(@"Invalid view returned from registry, expecting VRTVideoSurface, got: %@", view);
    } else {
      VRT360Video *component = (VRT360Video *)view;
      [component seekToTime:time];
    }
  }];
}

@end
