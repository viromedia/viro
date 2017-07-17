//
//  VRTLight.m
//  React
//
//  Created by Vik Advani on 1/7/16.
//  Copyright © 2016 Viro Media. All rights reserved.
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
