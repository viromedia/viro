//
//  VRTLightManager.m
//  React
//
//  Created by Vik Advani on 1/8/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTLight.h"
#import "VRTLightManager.h"

@implementation VRTLightManager

RCT_EXPORT_MODULE()
RCT_EXPORT_VIEW_PROPERTY(color, UIColor)
RCT_EXPORT_VIEW_PROPERTY(intensity, float)

@end

@implementation VRTDirectionalLightManager


RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(color, UIColor)
RCT_EXPORT_VIEW_PROPERTY(intensity, float)
RCT_EXPORT_VIEW_PROPERTY(direction, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(influenceBitMask, int)

// Shadow properties
RCT_EXPORT_VIEW_PROPERTY(castsShadow, BOOL)
RCT_EXPORT_VIEW_PROPERTY(shadowOpacity, float)
RCT_EXPORT_VIEW_PROPERTY(shadowOrthographicSize, float)
RCT_EXPORT_VIEW_PROPERTY(shadowOrthographicPosition, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(shadowMapSize, int)
RCT_EXPORT_VIEW_PROPERTY(shadowBias, float)
RCT_EXPORT_VIEW_PROPERTY(shadowNearZ, float)
RCT_EXPORT_VIEW_PROPERTY(shadowFarZ, float)

- (VRTView *)view
{
    return [[VRTDirectionalLight alloc] initWithBridge:self.bridge];
}

@end


@implementation VRTSpotLightManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(outerAngle, float)
RCT_EXPORT_VIEW_PROPERTY(innerAngle, float)
RCT_EXPORT_VIEW_PROPERTY(attenuationStartDistance, float)
RCT_EXPORT_VIEW_PROPERTY(attenuationEndDistance, float)
RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(color, UIColor)
RCT_EXPORT_VIEW_PROPERTY(intensity, float)
RCT_EXPORT_VIEW_PROPERTY(direction, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(influenceBitMask, int)

// Shadow properties
RCT_EXPORT_VIEW_PROPERTY(castsShadow, BOOL)
RCT_EXPORT_VIEW_PROPERTY(shadowOpacity, float)
RCT_EXPORT_VIEW_PROPERTY(shadowMapSize, int)
RCT_EXPORT_VIEW_PROPERTY(shadowBias, float)
RCT_EXPORT_VIEW_PROPERTY(shadowNearZ, float)
RCT_EXPORT_VIEW_PROPERTY(shadowFarZ, float)

- (VRTView *)view
{
    return [[VRTSpotLight alloc] initWithBridge:self.bridge];
}

@end


@implementation VRTAmbientLightManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(color, UIColor)
RCT_EXPORT_VIEW_PROPERTY(intensity, float)
RCT_EXPORT_VIEW_PROPERTY(temperature, float)
RCT_EXPORT_VIEW_PROPERTY(influenceBitMask, int)

- (VRTView *)view
{
    return [[VRTAmbientLight alloc] initWithBridge:self.bridge];
}

@end


@implementation VRTOmniLightManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(attenuationStartDistance, float)
RCT_EXPORT_VIEW_PROPERTY(attenuationEndDistance, float)
RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(color, UIColor)
RCT_EXPORT_VIEW_PROPERTY(intensity, float)
RCT_EXPORT_VIEW_PROPERTY(influenceBitMask, int)

- (VRTView *)view
{
    return [[VRTOmniLight alloc] initWithBridge:self.bridge];
}

@end


