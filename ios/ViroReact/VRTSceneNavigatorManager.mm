//
//  VRTSceneNavigatorManager.m
//  React
//
//  Created by Vik Advani on 3/11/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTSceneNavigatorManager.h"
#import "VRTSceneNavigator.h"
#import "ViroUIManager.h"

@implementation VRTSceneNavigatorManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(currentSceneIndex, NSInteger)
RCT_EXPORT_VIEW_PROPERTY(vrModeEnabled, BOOL)
RCT_EXPORT_VIEW_PROPERTY(apiKey, NSString)

- (VRTSceneNavigator *)view
{
  return [[VRTSceneNavigator alloc] initWithBridge:self.bridge];
}

RCT_EXPORT_METHOD(requestExit:(nonnull NSNumber *)reactTag)
{
  [self.bridge.viroUIManager addUIBlock:^(__unused ViroUIManager *uiManager, NSDictionary<NSNumber *, VRTView *> *viewRegistry) {
    VRTView *view = viewRegistry[reactTag];
    if (![view isKindOfClass:[VRTSceneNavigator class]]) {
      RCTLogError(@"Invalid view returned from registry, expecting VRTSceneNavigator, got: %@", view);
    } else {
      VRTSceneNavigator *component = (VRTSceneNavigator *)view;
      [component userDidRequestExitVR];
    }
  }];

}

@end
