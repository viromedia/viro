//
//  VRTLightingEnvironment.h
//  React
//
//  Copyright © 2018 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <React/RCTImageSource.h>
#import "VRTControl.h"
#import "VRTHDRImageAsyncLoader.h"

@interface VRTLightingEnvironment : VRTControl<VRTHDRImageAsyncLoaderEventDelegate>

@property (nonatomic, copy) RCTImageSource *source;
@property (nonatomic, copy) RCTDirectEventBlock onLoadStartViro;
@property (nonatomic, copy) RCTDirectEventBlock onLoadEndViro;
@property (nonatomic, copy) RCTDirectEventBlock onErrorViro;

- (instancetype)initWithBridge:(RCTBridge *)bridge;
- (void)updateSceneLightingEnvironment;

@end

