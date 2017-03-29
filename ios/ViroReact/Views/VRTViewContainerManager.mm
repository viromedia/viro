//
//  VRTViewContainerManager.m
//  React
//
//  Created by Vik Advani on 9/15/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTViewContainerManager.h"
#import "VRTNodeContainer.h"
#import "VRTNodeContainerShadowView.h"
#import "VRTNode.h"

@implementation VRTViewContainerManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scale, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotation, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(transformBehaviors, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(visible, BOOL)
RCT_EXPORT_VIEW_PROPERTY(onHoverViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onClickViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onFuseViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(timeToFuse, float)
RCT_EXPORT_VIEW_PROPERTY(canHover, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canClick, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canFuse, BOOL)
RCT_EXPORT_VIEW_PROPERTY(opacity, float)


- (VRTNodeContainerShadowView *)shadowView
{
  return [VRTNodeContainerShadowView new];
}

-(VRTNodeContainer *)view
{
  return [[VRTNodeContainer alloc] initWithBridge:self.bridge];
}

@end
