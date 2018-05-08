//
//  VRTARImageMarkerManager.m
//  ViroReact
//
//  Created by Andy Chu on 2/2/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#import "VRTARImageMarkerManager.h"
#import "VRTNodeContainerShadowView.h"
#import "VRTARImageMarker.h"

@implementation VRTARImageMarkerManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(target, NSString)
RCT_EXPORT_VIEW_PROPERTY(pauseUpdates, BOOL)
RCT_EXPORT_VIEW_PROPERTY(visible, BOOL)
RCT_EXPORT_VIEW_PROPERTY(onHoverViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onClickViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onFuseViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onDragViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(timeToFuse, float)
RCT_EXPORT_VIEW_PROPERTY(canHover, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canClick, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canFuse, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canDrag, BOOL)
RCT_EXPORT_VIEW_PROPERTY(onPinchViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onRotateViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(canPinch, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canRotate, BOOL)
RCT_EXPORT_VIEW_PROPERTY(opacity, float)
RCT_EXPORT_VIEW_PROPERTY(onCollisionViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(canCollide, BOOL)
RCT_EXPORT_VIEW_PROPERTY(viroTag, NSString)
RCT_EXPORT_VIEW_PROPERTY(onAnchorFoundViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onAnchorUpdatedViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onAnchorRemovedViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(ignoreEventHandling, BOOL)
RCT_EXPORT_VIEW_PROPERTY(dragType, NSString)
RCT_EXPORT_VIEW_PROPERTY(dragPlane, NSDictionary)
- (VRTNodeContainerShadowView *)shadowView
{
    return [VRTNodeContainerShadowView new];
}

- (VRTARImageMarker *)view
{
    return [[VRTARImageMarker alloc] initWithBridge:self.bridge];
}

@end
