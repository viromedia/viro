//
//  ViroParticleEmitter
//  ViroReact
//
//  Created by Andy Chu on 8/15/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <React/RCTBridge.h>
#import "VRTControl.h"
#import "VRTImageAsyncLoaderEventDelegate.h"

@interface VRTParticleEmitter : VRTControl <VRTImageAsyncLoaderEventDelegate>

@property (nonatomic, assign) float duration;
@property (nonatomic, assign) float delay;
@property (nonatomic, assign) BOOL loop;
@property (nonatomic, assign) BOOL run;
@property (nonatomic, assign) BOOL fixedToEmitter;
@property (nonatomic, copy, nullable) NSDictionary *image;
@property (nonatomic, copy, nullable) NSDictionary *spawnBehavior;
@property (nonatomic, copy, nullable) NSDictionary *particleAppearance;
@property (nonatomic, copy, nullable) NSDictionary *particlePhysics;

- (instancetype)initWithBridge:(RCTBridge *)bridge;

@end
