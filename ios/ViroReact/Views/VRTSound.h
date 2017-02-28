//
//  VRTSound.h
//  ViroReact
//
//  Created by Andy Chu on 1/26/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTView.h"
#import "VRTNode.h"

@interface VRTBaseSound : VRTView <VROSoundDelegate>

@property (nonatomic, assign) VROSoundType soundType;
@property (nonatomic, copy) NSDictionary *source;
@property (nonatomic, assign) BOOL paused;
@property (nonatomic, assign) float volume;
@property (nonatomic, assign) BOOL muted;
@property (nonatomic, assign) BOOL loop;
@property (nonatomic, copy) NSArray<NSNumber *> *rotation;
@property (nonatomic, copy) NSArray<NSNumber *> *position;
@property (nonatomic, assign) VROSoundRolloffModel rolloffModel;
@property (nonatomic, assign) float minDistance;
@property (nonatomic, assign) float maxDistance;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onFinishViro;

- (instancetype)initWithSoundType:(VROSoundType)soundType bridge:(RCTBridge *)bridge;

- (void)resetSound;
- (void)getSoundForName:(NSString *)name;
- (void)createSoundWithPath:(NSString *)path local:(BOOL)local;
// This function should reset all the props on the underlying native Sound object.
- (void)setNativeProps;
- (void)seekToTime:(NSInteger)time;

@end

@interface VRTSound : VRTBaseSound

- (instancetype)initWithBridge:(RCTBridge *)bridge;

@end

@interface VRTSoundField : VRTBaseSound

- (instancetype)initWithBridge:(RCTBridge *)bridge;

@end

@interface VRTSpatialSound : VRTBaseSound

- (instancetype)initWithBridge:(RCTBridge *)bridge;

@end

@interface VRTSpatialSoundWrapper : VRTNode

@property (nonatomic, copy) NSDictionary *source;
@property (nonatomic, assign) BOOL paused;
@property (nonatomic, assign) float volume;
@property (nonatomic, assign) BOOL muted;
@property (nonatomic, assign) BOOL loop;
@property (nonatomic, copy) NSArray<NSNumber *> *position;
@property (nonatomic, assign) VROSoundRolloffModel rolloffModel;
@property (nonatomic, assign) float minDistance;
@property (nonatomic, assign) float maxDistance;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onFinishViro;

- (instancetype)initWithBridge:(RCTBridge *)bridge;

@end
