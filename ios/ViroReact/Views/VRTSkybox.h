//
//  VRTSkybox.h
//  React
//
//  Created by Raj Advani on 10/5/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <React/RCTImageSource.h>
#import "VRTControl.h"
#import "VRTCubeMap.h"
#import "VRTImageAsyncLoaderEventDelegate.h"

@interface VRTSkybox : VRTControl<VRTImageAsyncLoaderEventDelegate>

@property (nonatomic) UIColor *color;
@property (nonatomic) VRTCubeMap *source;
@property (nonatomic, copy) RCTDirectEventBlock onViroSkyBoxLoadStart;
@property (nonatomic, copy) RCTDirectEventBlock onViroSkyBoxLoadEnd;

- (instancetype)initWithBridge:(RCTBridge *)bridge;

@end
