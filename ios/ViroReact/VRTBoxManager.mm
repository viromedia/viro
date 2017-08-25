//
//  VROBoxManager.m
//  React
//
//  Created by Vik Advani on 12/15/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#import "VRTBoxManager.h"
#import "VRTBox.h"


@implementation VRTBoxManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(width, float)
RCT_EXPORT_VIEW_PROPERTY(height, float)
RCT_EXPORT_VIEW_PROPERTY(length, float)
RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scale, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotation, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotationPivot, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scalePivot, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(materials, NSArray<NSString *>)
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
RCT_EXPORT_VIEW_PROPERTY(opacity, float)
RCT_EXPORT_VIEW_PROPERTY(transformBehaviors, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(physicsBody, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(onCollidedViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(canCollide, BOOL)
RCT_EXPORT_VIEW_PROPERTY(viroTag, NSString)
RCT_EXPORT_VIEW_PROPERTY(onNativeTransformDelegateViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(hasTransformDelegate, BOOL)
RCT_EXPORT_VIEW_PROPERTY(lightBitMask, int)
RCT_EXPORT_VIEW_PROPERTY(shadowCastingBitMask, int)

- (VRTView *)view
{
    return [[VRTBox alloc] initWithBridge:self.bridge];
}


@end
