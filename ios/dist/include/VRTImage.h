//
//  VRTImageSurface.h
//  React
//
//  Created by Vik Advani on 3/3/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <React/RCTBridge.h>
#import <React/RCTImageSource.h>
#import <React/RCTConvert.h>
#import "VRTControl.h"
#import "VRTImageAsyncLoaderEventDelegate.h"

@interface VRTImage : VRTControl<VRTImageAsyncLoaderEventDelegate>

-(instancetype)initWithBridge:(RCTBridge *)bridge;

@property (nonatomic, assign) float width;
@property (nonatomic, assign) float height;
@property (nonatomic, copy) RCTImageSource *source;
@property (nonatomic, copy) UIImage *placeholderSource;
@property (nonatomic, assign) bool mipmap;
@property (nonatomic, assign) VROTextureInternalFormat format;
@property (nonatomic, copy) RCTDirectEventBlock onLoadStartViro;
@property (nonatomic, copy) RCTDirectEventBlock onLoadEndViro;

@end

@interface RCTConvert (VRTImage)

+ (VROTextureInternalFormat)VROTextureInternalFormat:(id)json;

@end
