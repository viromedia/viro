//
//  VRTHDRImageAsyncLoader.h
//  React
//
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VRTHDRImageAsyncLoader_h
#define VRTHDRImageAsyncLoader_h
#import <ViroKit/ViroKit.h>
@class VRTHDRImageAsyncLoader;

/*
 Callback protocol to be implemented for listening for HDR Image Async loading delegates.
 */
@protocol VRTHDRImageAsyncLoaderEventDelegate<NSObject>
- (void)hdrImageLoaderDidStart:(VRTHDRImageAsyncLoader *)loader;
- (void)hdrImageLoaderDidEnd:(VRTHDRImageAsyncLoader *)loader hdrTexture:(std::shared_ptr<VROTexture>)texture;
@end

/*

 */
@interface VRTHDRImageAsyncLoader : NSObject
@property (readwrite, nonatomic, copy) NSString *tag;
@property (nonatomic, weak, nullable) id<VRTHDRImageAsyncLoaderEventDelegate> delegate;
- (instancetype)initWithDelegate:(id<VRTHDRImageAsyncLoaderEventDelegate>)delegate;
- (void)loadHdrImage:(RCTImageSource *)imageSource;
- (void)cancel;
- (BOOL)isHdrSource:(RCTImageSource *)imageSource;
@end

#endif /* VRTHDRImageAsyncLoader_h */
