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
RCT_EXPORT_VIEW_PROPERTY(onTap, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onGaze, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(opacity, float)


- (VRTNodeContainerShadowView *)shadowView
{
  return [VRTNodeContainerShadowView new];
}

-(VRTNodeContainer *)view
{
  VRTNodeContainer *node = [[VRTNodeContainer alloc] initWithBridge:self.bridge];
  node.delegate = self;
  return node;
}

@end
