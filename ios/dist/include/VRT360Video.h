//
//  VRT360Video.h
//  React
//
//  Created by Vik Advani on 1/28/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTControl.h"

@interface VRT360Video : VRTControl <VROVideoDelegate>

@property (nonatomic, copy) NSDictionary *source;
@property (nonatomic, assign) BOOL paused;
@property (nonatomic, assign) BOOL loop;
@property (nonatomic, assign) BOOL muted;
@property (nonatomic, assign) float volume;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onBufferStartViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onBufferEndViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onFinishViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onUpdateTimeViro;
@property (nonatomic, copy) RCTDirectEventBlock onErrorViro;
@property (nonatomic, copy, nullable) NSString *stereoMode;

- (instancetype)initWithBridge:(RCTBridge *)bridge;
- (void)seekToTime:(float)time;

@end
