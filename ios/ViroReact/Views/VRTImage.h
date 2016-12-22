//
//  VRTImageSurface.h
//  React
//
//  Created by Vik Advani on 3/3/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTControl.h"
#import "RCTBridge.h"
#import "RCTImageSource.h"
#import "VRTImageAsyncLoaderEventDelegate.h"

@interface VRTImage : VRTControl<VRTImageAsyncLoaderEventDelegate>

-(instancetype)initWithBridge:(RCTBridge *)bridge;

@property (nonatomic, assign) float width;
@property (nonatomic, assign) float height;
@property (nonatomic, copy) RCTImageSource *source;
@property (nonatomic, copy) UIImage *placeHolderSource;
@property (nonatomic, copy) RCTDirectEventBlock onLoadStartViro;
@property (nonatomic, copy) RCTDirectEventBlock onLoadEndViro;


@end
