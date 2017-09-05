//
//  VRT3DObjectManager.m
//  React
//
//  Created by vik.advani on 4/18/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRT3DObjectManager.h"
#import "VRT3DObject.h"

@implementation VRT3DObjectManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(source, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(animation, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(onAnimationStartViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onAnimationFinishViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotation, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scale, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotationPivot, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scalePivot, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(materials, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(visible, BOOL)
RCT_EXPORT_VIEW_PROPERTY(opacity, float)
RCT_EXPORT_VIEW_PROPERTY(onHoverViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onClickViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onFuseViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onDragViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onPinchViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onRotateViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(timeToFuse, float)
RCT_EXPORT_VIEW_PROPERTY(canHover, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canClick, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canFuse, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canDrag, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canPinch, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canRotate, BOOL)
RCT_EXPORT_VIEW_PROPERTY(transformBehaviors, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(highAccuracyGaze, BOOL)
RCT_EXPORT_VIEW_PROPERTY(onLoadStartViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onLoadEndViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onErrorViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(physicsBody, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(onCollidedViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(canCollide, BOOL)
RCT_EXPORT_VIEW_PROPERTY(viroTag, NSString)
RCT_EXPORT_VIEW_PROPERTY(onNativeTransformDelegateViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(hasTransformDelegate, BOOL)
RCT_EXPORT_VIEW_PROPERTY(lightBitMask, int)
RCT_EXPORT_VIEW_PROPERTY(shadowCastingBitMask, int)
RCT_EXPORT_VIEW_PROPERTY(ignoreEventHandling, BOOL)

- (VRT3DObject *)view
{
    return [[VRT3DObject alloc] initWithBridge:self.bridge];
}

@end
