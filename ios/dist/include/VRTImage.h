//
//  VRTImage.h
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
#import "VRTUIImageWrapper.h"
#import "VRTImageAsyncLoaderEventDelegate.h"

enum class VROImageResizeMode {
  ScaleToFill,
  ScaleToFit,
  StretchToFill
};

enum class VROImageClipMode {
  ClipToBounds,
  None
};

@interface VRTImage : VRTControl<VRTImageAsyncLoaderEventDelegate>

-(instancetype)initWithBridge:(RCTBridge *)bridge;

@property (nonatomic, assign) float width;
@property (nonatomic, assign) float height;
@property (nonatomic, copy) RCTImageSource *source;
@property (nonatomic, assign) VROImageResizeMode resizeMode;
@property (nonatomic, assign) VROImageClipMode imageClipMode;
@property (nonatomic, copy) VRTUIImageWrapper *placeholderSource;
@property (nonatomic, assign) bool mipmap;
@property (nonatomic, assign) VROTextureInternalFormat format;
@property (nonatomic, copy) RCTDirectEventBlock onLoadStartViro;
@property (nonatomic, copy) RCTDirectEventBlock onLoadEndViro;
@property (nonatomic, copy) RCTDirectEventBlock onErrorViro;
@property (nonatomic, copy, nullable) NSString *stereoMode;
@property (readwrite, nonatomic) std::shared_ptr<VROTexture> texture;
@property (nonatomic, assign) bool imageNeedsDownload;

- (void)updatePlaceholderProps:(NSArray<NSString *> *)changedProps;
- (void)updateMainImage:(float)width height:(float)height;
@end

@interface RCTConvert (VRTImage)

+ (VROTextureInternalFormat)VROTextureInternalFormat:(id)json;
+ (VROImageResizeMode)VROImageResizeMode:(id)json;
+ (VROImageClipMode)VROImageClipMode:(id)json;

@end
