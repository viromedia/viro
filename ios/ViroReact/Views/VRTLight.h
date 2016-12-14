//
//  VRTLight.h
//  React
//
//  Created by Vik Advani on 1/7/16.
//  Copyright © 2016 Facebook. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTView.h"

@interface VRTLight :VRTView

@property (nonatomic, copy) NSArray<NSNumber *> *position;
@property (nonatomic, copy) NSArray<NSNumber *> *direction;

- (instancetype)initWithLightType:(VROLightType)lightType bridge:(RCTBridge *)bridge;
- (std::shared_ptr<VROLight>)light;
- (void)setColor:(NSArray<NSNumber *> *)color;

@end

@interface VRTSpotLight: VRTLight
- (instancetype)initWithBridge:(RCTBridge *)bridge;
- (void)setPosition:(NSArray<NSNumber *> *)position;
- (void)setDirection:(NSArray<NSNumber *> *)direction;
- (void)setOuterAngle:(float)outerAngle;
- (void)setInnerAngle:(float)innerAngle;
- (void)setAttenuationEndDistance:(float)endDistance;
- (void)setAttenuationStartDistance:(float)startDistance;
@end

@interface VRTAmbientLight: VRTLight
- (instancetype)initWithBridge:(RCTBridge *)bridge;
@end

@interface VRTDirectionalLight: VRTLight
- (instancetype)initWithBridge:(RCTBridge *)bridge;
- (void)setDirection:(NSArray<NSNumber *> *)direction;
@end

@interface VRTOmniLight: VRTLight
- (instancetype)initWithBridge:(RCTBridge *)bridge;
- (void)setPosition:(NSArray<NSNumber *> *)position;
- (void)setAttenuationEndDistance:(float)endDistance;
- (void)setAttenuationStartDistance:(float)startDistance;
@end
