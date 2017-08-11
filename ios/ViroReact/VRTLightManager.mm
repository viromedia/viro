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
RCT_EXPORT_VIEW_PROPERTY(direction, NSNumberArray)

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

- (VRTView *)view
{
    return [[VRTSpotLight alloc] initWithBridge:self.bridge];
}

@end


@implementation VRTAmbientLightManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(color, UIColor)
RCT_EXPORT_VIEW_PROPERTY(intensity, float)

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

- (VRTView *)view
{
    return [[VRTOmniLight alloc] initWithBridge:self.bridge];
}

@end


