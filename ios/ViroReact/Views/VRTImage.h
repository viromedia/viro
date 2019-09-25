//
//  VRTImage.h
//  React
//
//  Created by Vik Advani on 3/3/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
