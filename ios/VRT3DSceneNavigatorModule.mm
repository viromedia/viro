//
//  VRTSceneNavigatorModule.m
//  ViroReact
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <React/RCTUIManager.h>
#import <React/RCTUIManagerUtils.h>
#import "VRT3DSceneNavigatorModule.h"
#import "VRT3DSceneNavigator.h"

@implementation VRT3DSceneNavigatorModule
@synthesize bridge = _bridge;

RCT_EXPORT_MODULE()

- (dispatch_queue_t)methodQueue {
  return RCTGetUIManagerQueue();
}

RCT_EXPORT_METHOD(requestExitVr:(nonnull NSNumber *)sceneNavTag) {
  [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
    NSLog(@"User Exit VR VRTSceneNav Module RCT_EXPORT_METHOD");
    
    UIView *sceneNavigator = viewRegistry[sceneNavTag];
    
    if (![sceneNavigator isKindOfClass:[VRT3DSceneNavigator class]]) {
      RCTLogError(@"Invalid view returned when requestingExitVR: expected VRT3DSceneNavigator, got [%@]", sceneNavigator);
    }
    else {
      NSLog(@"User Exit VR VRTSceneNav Module");
      VRT3DSceneNavigator *nav = (VRT3DSceneNavigator *)sceneNavigator;
      [nav userDidRequestExitVR];
    }
  }];
}

RCT_EXPORT_METHOD(recenterTracking:(nonnull NSNumber *)sceneNavTag) {
  [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
    UIView *sceneNavigator = viewRegistry[sceneNavTag];
    
    if (![sceneNavigator isKindOfClass:[VRT3DSceneNavigator class]]) {
      RCTLogError(@"Invalid view returned when recenterTracking: expected VRTSceneNavigator, got [%@]", sceneNavigator);
    }
    else {
      NSLog(@"Viro - recentering tracking.");
      VRT3DSceneNavigator *nav = (VRT3DSceneNavigator *)sceneNavigator;
      [nav recenterTracking];
    }
  }];
}

@end

