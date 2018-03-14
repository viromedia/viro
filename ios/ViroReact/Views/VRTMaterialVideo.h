//
//  VRTMaterialVideo.h
//  ViroReact
//
//  Created by vik.advani on 3/8/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTView.h"
#import "VRTMaterialChangedDelegate.h"

@interface VRTMaterialVideo : VRTView<VROVideoDelegate, VRTMaterialChangedDelegate>

@property (nonatomic, assign) BOOL paused;
@property (nonatomic, assign) BOOL loop;
@property (nonatomic, assign) BOOL muted;
@property (nonatomic, assign) float volume;
@property (nonatomic, copy) NSString * material;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onBufferStartViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onBufferEndViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onFinishViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onUpdateTimeViro;
@property (nonatomic, copy) RCTDirectEventBlock onErrorViro;

- (instancetype)initWithBridge:(RCTBridge *)bridge;
- (void)seekToTime:(float)time;
@end
