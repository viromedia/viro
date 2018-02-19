//
//  VRT360Image.h
//  React
//
//  Created by Vik Advani on 3/3/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <React/RCTImageSource.h>
#import "VRTControl.h"
#import "VRTImageAsyncLoaderEventDelegate.h"
#import "VRTHDRImageAsyncLoader.h"

@interface VRT360Image : VRTControl<VRTImageAsyncLoaderEventDelegate, VRTHDRImageAsyncLoaderEventDelegate>

@property (nonatomic, copy) RCTImageSource *source;
@property (nonatomic, copy) RCTDirectEventBlock onLoadStartViro;
@property (nonatomic, copy) RCTDirectEventBlock onLoadEndViro;
@property (nonatomic, copy) RCTDirectEventBlock onErrorViro;
@property (nonatomic, assign) VROTextureInternalFormat format;
@property (nonatomic, copy, nullable) NSString *stereoMode;

- (instancetype)initWithBridge:(RCTBridge *)bridge;

@end
