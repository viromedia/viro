//
//  VROSceneManager.m
//  React
//
//  Created by Vik Advani on 11/3/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#import "VROSceneManager.h"
#import "RCTConvert.h"
#include "VRTScene.h"
#include "VRTSceneShadowView.h"
#import "ViroUIManager.h"
#import <ViroKit/ViroKit.h>

@implementation VROSceneManager{
  
}

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(onGaze, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onTap, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(reticleEnabled, BOOL)

- (VRTView *)view
{
  VRTScene * view = [[VRTScene alloc] initWithBridge:self.bridge];
  view.delegate = self;
  return view;
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
