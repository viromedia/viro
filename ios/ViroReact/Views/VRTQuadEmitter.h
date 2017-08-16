//
//  VRTQuadEmitter.h
//  ViroReact
//
//  Created by Andy Chu on 8/15/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <React/RCTBridge.h>
#import "VRTControl.h"
#import "VRTImageAsyncLoaderEventDelegate.h"

@interface VRTQuadEmitter : VRTControl <VRTImageAsyncLoaderEventDelegate>

@property (nonatomic, assign) float duration;
@property (nonatomic, assign) float delay;
@property (nonatomic, assign) BOOL loop;
@property (nonatomic, assign) BOOL run;
@property (nonatomic, assign) BOOL fixedToEmitter;
@property (nonatomic, copy, nullable) NSDictionary *quad;
@property (nonatomic, copy, nullable) NSDictionary *spawnModifier;
@property (nonatomic, copy, nullable) NSDictionary *appearanceModifier;
@property (nonatomic, copy, nullable) NSDictionary *physicsModifier;

- (instancetype)initWithBridge:(RCTBridge *)bridge;

@end
