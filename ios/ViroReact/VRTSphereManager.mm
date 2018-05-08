//
//  VRTSphereManager.m
//  React
//
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTSphereManager.h"
#import "VRTSphere.h"

@implementation VRTSphereManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scale, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotation, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotationPivot, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scalePivot, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(materials, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(visible, BOOL)
RCT_EXPORT_VIEW_PROPERTY(animation, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(onAnimationStartViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onAnimationFinishViro, RCTDirectEventBlock)
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
RCT_EXPORT_VIEW_PROPERTY(facesOutward, BOOL)
RCT_EXPORT_VIEW_PROPERTY(widthSegmentCount, NSUInteger)
RCT_EXPORT_VIEW_PROPERTY(heightSegmentCount, NSUInteger)
RCT_EXPORT_VIEW_PROPERTY(radius, float)
RCT_EXPORT_VIEW_PROPERTY(transformBehaviors, NSArray<NSString *>)
RCT_EXPORT_VIEW_PROPERTY(highAccuracyGaze, BOOL)
RCT_EXPORT_VIEW_PROPERTY(physicsBody, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(onCollisionViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(canCollide, BOOL)
RCT_EXPORT_VIEW_PROPERTY(viroTag, NSString)
RCT_EXPORT_VIEW_PROPERTY(onNativeTransformDelegateViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(hasTransformDelegate, BOOL)
RCT_EXPORT_VIEW_PROPERTY(lightReceivingBitMask, int)
RCT_EXPORT_VIEW_PROPERTY(shadowCastingBitMask, int)
RCT_EXPORT_VIEW_PROPERTY(ignoreEventHandling, BOOL)
RCT_EXPORT_VIEW_PROPERTY(dragType, NSString)
RCT_EXPORT_VIEW_PROPERTY(dragPlane, NSDictionary)
- (VRTView *)view
{
    return [[VRTSphere alloc] initWithBridge:self.bridge];
}


@end
