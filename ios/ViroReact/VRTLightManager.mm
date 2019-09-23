//
//  VRTLightManager.m
//  React
//
//  Created by Vik Advani on 1/8/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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


