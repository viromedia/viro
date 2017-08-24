//
//  VRTImageAsyncLoader.h
//  React
//
//  Created by Vik Advani on 9/28/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <React/RCTImageSource.h>
#import "VRTImageAsyncLoaderEventDelegate.h"


@interface VRTImageAsyncLoader : NSObject

@property (readwrite, nonatomic, copy) NSString *tag;
@property (nonatomic, weak, nullable) id<VRTImageAsyncLoaderEventDelegate> delegate;

- (instancetype)initWithDelegate:(id<VRTImageAsyncLoaderEventDelegate>)delegate;
- (void)loadImage:(RCTImageSource *)imageSource;
- (void)cancel;

@end


