//
//  VRTQuadEmitterManager.m
//  ViroReact
//
//  Created by Andy Chu on 8/15/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import "VRTQuadEmitterManager.h"
#import "VRTNodeContainerShadowView.h"
#import "VRTQuadEmitter.h"

@implementation VRTQuadEmitterManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotation, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scale, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(scalePivot, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(rotationPivot, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(duration, float)
RCT_EXPORT_VIEW_PROPERTY(delay, float)
RCT_EXPORT_VIEW_PROPERTY(loop, BOOL)
RCT_EXPORT_VIEW_PROPERTY(run, BOOL)
RCT_EXPORT_VIEW_PROPERTY(visible, BOOL)
RCT_EXPORT_VIEW_PROPERTY(fixedToEmitter, BOOL)
RCT_EXPORT_VIEW_PROPERTY(quad, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(spawnModifier, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(appearanceModifier, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(physicsModifier, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(onNativeTransformDelegateViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(hasTransformDelegate, BOOL)

- (VRTNodeContainerShadowView *)shadowView
{
    return [VRTNodeContainerShadowView new];
}

- (VRTQuadEmitter *)view
{
    return [[VRTQuadEmitter alloc] initWithBridge:self.bridge];
}

@end
