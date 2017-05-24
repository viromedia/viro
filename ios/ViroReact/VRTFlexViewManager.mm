//
//  VRTFlexViewManager.m
//  React
//
//  Created by Vik Advani on 5/9/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTFlexViewManager.h"
#import "VRTFlexView.h"
#import "VRTShadowFlexView.h"

@implementation VRTFlexViewManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(height, float)
RCT_EXPORT_VIEW_PROPERTY(width, float)

RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotation, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scale, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotationPivot, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scalePivot, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(opacity, float)

RCT_EXPORT_VIEW_PROPERTY(materials, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(onHoverViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onClickViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onFuseViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(timeToFuse, float)
RCT_EXPORT_VIEW_PROPERTY(canHover, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canClick, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canFuse, BOOL)
RCT_EXPORT_VIEW_PROPERTY(transformBehaviors, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(visible, BOOL)
RCT_REMAP_VIEW_PROPERTY(backgroundColor, flexBackgroundColor, UIColor) //backgroundColor is reserved

- (VRTFlexView *)view
{
  return [[VRTFlexView alloc] initWithBridge:self.bridge];
}

- (VRTShadowFlexView *)shadowView
{
  return [VRTShadowFlexView new];
}

-(BOOL)isRootFlexBoxPanel {
  return YES;
}

@end
