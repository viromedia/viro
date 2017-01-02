//
//  VRTSceneManager.m
//  React
//
//  Created by Vik Advani on 11/3/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#import "VRTSceneManager.h"
#import "RCTConvert.h"
#include "VRTScene.h"
#include "VRTSceneShadowView.h"
#import "ViroUIManager.h"
#import <ViroKit/ViroKit.h>

@implementation VRTSceneManager {
  
}

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(onGazeViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onTapViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(canTap, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canGaze, BOOL)
RCT_EXPORT_VIEW_PROPERTY(reticleEnabled, BOOL)

- (VRTView *)view
{
  return [[VRTScene alloc] initWithBridge:self.bridge];
}

- (VRTSceneShadowView *)shadowView
{
  return [VRTSceneShadowView new];
}

RCT_EXPORT_METHOD(getCameraPosition:(nonnull NSNumber *)reactTag
                  resolve:(RCTPromiseResolveBlock)resolve
                  reject:(RCTPromiseRejectBlock)reject) {
  [self.bridge.viroUIManager addUIBlock:^(__unused ViroUIManager *uiManager, NSDictionary<NSNumber *, VRTView *> *viewRegistry) {
    VRTView *view = viewRegistry[reactTag];
    if (![view isKindOfClass:[VRTScene class]]) {
      RCTLogError(@"Invalid view returned from registry, expecting VRTScene, got: %@", view);
    } else {
      resolve(((VRTScene *)view).cameraPosition);
    }
  }];
}

@end
