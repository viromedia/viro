//
//  VRTLight.h
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

#import <Foundation/Foundation.h>
#import "VRTView.h"

@interface VRTLight :VRTView

@property (nonatomic, copy) NSArray<NSNumber *> *position;
@property (nonatomic, copy) NSArray<NSNumber *> *direction;

- (instancetype)initWithLightType:(VROLightType)lightType bridge:(RCTBridge *)bridge;
- (std::shared_ptr<VROLight>)light;
- (void)setColor:(NSArray<NSNumber *> *)color;
- (void)setIntensity:(float)intensity;
- (void)setTemperature:(float)temperature;
- (void)setInfluenceBitMask:(int)bitMask;
- (void)setCastsShadow:(BOOL)castsShadow;

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
