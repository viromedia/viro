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
#import "RCTUIManager.h"
#import "VRTCamera.h"
#import "RCTBridge.h"
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

@end
