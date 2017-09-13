//
//  VRTParticleEmitterManager.m
//  ViroReact
//
//  Created by Andy Chu on 8/15/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import "VRTParticleEmitterManager.h"
#import "VRTNodeContainerShadowView.h"
#import "VRTParticleEmitter.h"

@implementation VRTParticleEmitterManager

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
RCT_EXPORT_VIEW_PROPERTY(image, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(spawnBehavior, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(particleAppearance, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(particlePhysics, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(onNativeTransformDelegateViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(hasTransformDelegate, BOOL)

- (VRTNodeContainerShadowView *)shadowView
{
    return [VRTNodeContainerShadowView new];
}

- (VRTParticleEmitter *)view
{
    return [[VRTParticleEmitter alloc] initWithBridge:self.bridge];
}

@end
