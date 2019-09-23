//
//  VRTSound.h
//  ViroReact
//
//  Created by Andy Chu on 1/26/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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
@property (nonatomic, assign) NSString *rolloffModel;
@property (nonatomic, assign) float minDistance;
@property (nonatomic, assign) float maxDistance;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onFinishViro;
@property (nonatomic, copy) RCTDirectEventBlock onErrorViro;

- (instancetype)initWithSoundType:(VROSoundType)soundType bridge:(RCTBridge *)bridge;

- (void)resetSound;
- (void)getSoundForName:(NSString *)name;
- (void)createSoundWithPath:(NSString *)path resourceType:(VROResourceType)resourceType;
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
@property (nonatomic, assign) NSString *rolloffModel;
@property (nonatomic, assign) float minDistance;
@property (nonatomic, assign) float maxDistance;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onFinishViro;
@property (nonatomic, copy) RCTDirectEventBlock onErrorViro;

- (instancetype)initWithBridge:(RCTBridge *)bridge;

@end
