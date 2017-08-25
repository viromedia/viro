//
//  VRTPhotoLibraryAsyncLoader.h
//  ViroReact
//
//  Created by vik.advani on 8/25/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VRTPhotoLibraryAsyncLoader_h
#define VRTPhotoLibraryAsyncLoader_h

#import <Foundation/Foundation.h>
#import <React/RCTImageSource.h>
#import "VRTImageAsyncLoaderEventDelegate.h"


@interface VRTPhotoLibraryAsyncLoader : NSObject

@property (readwrite, nonatomic, copy) NSString *tag;
@property (nonatomic, weak, nullable) id<VRTImageAsyncLoaderEventDelegate> delegate;

- (instancetype)initWithDelegate:(id<VRTImageAsyncLoaderEventDelegate>)delegate;
- (void)loadImage:(RCTImageSource *)imageSource;
- (BOOL)canLoadImageURL:(NSURL *)requestURL;

@end

#endif /* VRTPhotoLibraryAsyncLoader_h */
