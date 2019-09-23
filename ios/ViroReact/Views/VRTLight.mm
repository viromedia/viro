//
//  VRTLight.m
//  React
//
//  Created by Vik Advani on 1/7/16.
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

#import <ViroKit/ViroKit.h>
#import "VRTLight.h"
#import "VRTUtils.h"

@interface VRTLight ()

@property (readwrite, nonatomic) std::shared_ptr<VROLight> light;

@end

@implementation VRTLight

@synthesize position = _position;

- (instancetype)initWithLightType:(VROLightType)lightType bridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _light = std::make_shared<VROLight>(lightType);
    }
    
    return self;
}

-(std::shared_ptr<VROLight>) light {
    return _light;
}

- (void)setColor:(UIColor *)color {
    CGFloat floatColor[4];
    [color getRed:&floatColor[0] green:&floatColor[1] blue:&floatColor[2] alpha:&floatColor[3]];
    
    //Possible loss of precision casting to float, if CGFloat is cast as double for 64 bit devices.
    _light->setColor({static_cast<float>(floatColor[0]), static_cast<float>(floatColor[1]), static_cast<float>(floatColor[2])});
}

- (void)setIntensity:(float)intensity {
    _light->setIntensity(intensity);
}

- (void)setTemperature:(float)temperature {
    _light->setTemperature(temperature);
}

- (void)setInfluenceBitMask:(int)bitMask {
    _light->setInfluenceBitMask(bitMask);
}

- (void)setCastsShadow:(BOOL)castsShadow {
    _light->setCastsShadow(castsShadow);
}

- (void)setShadowOpacity:(float)shadowOpacity {
    _light->setShadowOpacity(shadowOpacity);
}

- (void)setShadowOrthographicSize:(float)orthographicSize {
    _light->setShadowOrthographicSize(orthographicSize);
}

- (void)setShadowMapSize:(int)shadowMapSize {
    _light->setShadowMapSize(shadowMapSize);
}

- (void)setShadowBias:(float)shadowBias {
    _light->setShadowBias(shadowBias);
}

- (void)setShadowNearZ:(float)nearZ {
    _light->setShadowNearZ(nearZ);
}

- (void)setShadowFarZ:(float)farZ {
    _light->setShadowFarZ(farZ);
}

@end

@implementation VRTAmbientLight
- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithLightType:VROLightType::Ambient bridge:bridge];
    return self;
}

@end

#pragma mark - Directional Light

@implementation VRTDirectionalLight
- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithLightType:VROLightType::Directional bridge:bridge];
    return self;
}

- (void)setDirection:(NSArray<NSNumber *> *)direction {
    [super setDirection:[direction copy]];
    
    float directionValues[3];
    populateFloatArrayFromNSArray(direction, directionValues, 3);
    self.light->setDirection({directionValues[0], directionValues[1], directionValues[2]});
}

- (void)setShadowOrthographicPosition:(NSArray<NSNumber *> *)orthographicPosition {
    float position[3];
    populateFloatArrayFromNSArray(orthographicPosition, position, 3);
    self.light->setPosition({ position[0], position[1], position[2] });
}

@end

#pragma mark - Omni Light

@implementation VRTOmniLight
- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithLightType:VROLightType::Omni bridge:bridge];
    return self;
}

- (void)setPosition:(NSArray<NSNumber *> *)position {
    [super setPosition:[position copy]];
    
    float positionValues[3];
    populateFloatArrayFromNSArray(position, positionValues, 3);
    self.light->setPosition({positionValues[0], positionValues[1], positionValues[2]});
}

- (void)setAttenuationEndDistance:(float) endDistance {
    self.light->setAttenuationEndDistance(endDistance);
}

- (void)setAttenuationStartDistance:(float) startDistance {
    self.light->setAttenuationStartDistance(startDistance);
}

@end

#pragma mark - Spot Light

@implementation VRTSpotLight
- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithLightType:VROLightType::Spot bridge:bridge];
    return self;
}

- (void)setPosition:(NSArray<NSNumber *> *)position {
    [super setPosition:[position copy]];
    
    float positionValues[3];
    populateFloatArrayFromNSArray(position, positionValues, 3);
    self.light->setPosition({positionValues[0], positionValues[1], positionValues[2]});
}

- (void)setDirection:(NSArray<NSNumber *> *)direction {
    [super setDirection:[direction copy]];
    
    float directionValues[3];
    populateFloatArrayFromNSArray(direction, directionValues, 3);
    self.light->setDirection({directionValues[0], directionValues[1], directionValues[2]});
}

- (void)setOuterAngle:(float)outerAngle {
    self.light->setSpotOuterAngle(outerAngle);
}

- (void)setInnerAngle:(float)innerAngle{
    self.light->setSpotInnerAngle(innerAngle);
}

- (void)setAttenuationEndDistance:(float)endDistance {
    self.light->setAttenuationEndDistance(endDistance);
}

- (void)setAttenuationStartDistance:(float)startDistance {
    self.light->setAttenuationStartDistance(startDistance);
}

@end
