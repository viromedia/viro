//
//  VRTVRSceneNavigatorManager.m
//  ViroReact
//
//  Created by Manish Bodhankar on 3/30/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#import <React/RCTUIManager.h>
#import "VRTVRSceneNavigatorManager.h"
#import "VRTVRSceneNavigator.h"


@implementation VRTVRSceneNavigatorManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(currentSceneIndex, NSInteger)
RCT_EXPORT_VIEW_PROPERTY(vrModeEnabled, BOOL)
RCT_EXPORT_VIEW_PROPERTY(apiKey, NSString)
RCT_EXPORT_VIEW_PROPERTY(onExitViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(hdrEnabled, BOOL)
RCT_EXPORT_VIEW_PROPERTY(pbrEnabled, BOOL)
RCT_EXPORT_VIEW_PROPERTY(bloomEnabled, BOOL)
RCT_EXPORT_VIEW_PROPERTY(shadowsEnabled, BOOL)
RCT_EXPORT_VIEW_PROPERTY(multisamplingEnabled, BOOL)

- (VRTVRSceneNavigator *)view
{
  return [[VRTVRSceneNavigator alloc] initWithBridge:self.bridge];
}

RCT_EXPORT_METHOD(requestExit:(nonnull NSNumber *)reactTag)
{
  [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
    VRTView *view = (VRTView *)viewRegistry[reactTag];
    if (![view isKindOfClass:[VRTVRSceneNavigator class]]) {
      RCTLogError(@"Invalid view returned from registry, expecting VRTSceneNavigator, got: %@", view);
    } else {
      VRTVRSceneNavigator *component = (VRTVRSceneNavigator *)view;
      [component userDidRequestExitVR];
    }
  }];
}

@end

