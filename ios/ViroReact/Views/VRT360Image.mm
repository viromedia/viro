//
//  VRT360Image.m
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

#import <React/RCTConvert.h>
#import <React/RCTUtils.h>
#import "VRTUtils.h"
#import "VRT360Image.h"
#import "VRTImageAsyncLoader.h"
#import "VRTPhotoLibraryAsyncLoader.h"

@interface RCTImageSource (Viro)

@property (nonatomic, assign) BOOL packagerAsset;

@end

@implementation VRT360Image {
    std::shared_ptr<VROTexture> _sphereTexture;
    BOOL _sphereTextureAddedToScene;
    BOOL _imageNeedsDownload;
    VRTImageAsyncLoader *_imageAsyncLoader;
    VRTPhotoLibraryAsyncLoader *_assetLoader;
    VRTHDRImageAsyncLoader *_hdrLoader;
    NSString *_stereoMode;
}

@synthesize onLoadStartViro = _onLoadStartViro;
@synthesize onLoadEndViro = _onLoadEndViro;
@synthesize source = _source;
@synthesize rotation = _rotation;

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _sphereTextureAddedToScene = NO;
        _imageNeedsDownload = NO;
        _format = VROTextureInternalFormat::RGBA8;
        _imageAsyncLoader = [[VRTImageAsyncLoader alloc] initWithDelegate:self];
        _assetLoader = [[VRTPhotoLibraryAsyncLoader alloc] initWithDelegate:self];
        _hdrLoader= [[VRTHDRImageAsyncLoader alloc] initWithDelegate:self];
    }
    return self;
}

// Loading from disk and network should both be async. Currently disk loading will block
// Reference RCTImage package for how to do image loaders properly.
- (void)setSource:(RCTImageSource *)source {
    _source = source;
    _imageNeedsDownload = YES;
}

- (void)setOnLoadStartViro:(RCTDirectEventBlock)onLoadStart {
    _onLoadStartViro = onLoadStart;
}

- (void)setOnLoadEndViro:(RCTDirectEventBlock)onLoadEnd {
    _onLoadEndViro = onLoadEnd;
}

- (void)setFormat:(VROTextureInternalFormat)format {
    _format = format;
    _imageNeedsDownload = YES;
}

- (void)setStereoMode:(NSString *)mode;{
    _stereoMode = mode;
}

- (void)setRotation:(NSArray<NSNumber *> *)rotation {
    _rotation = [rotation copy];
    if (_sphereTextureAddedToScene) {
        float rotationValues[3] = {0.0f, 0.0f, 0.0f};
        populateFloatArrayFromNSArray(_rotation, rotationValues, 3);
        self.node->getParentPortal()->setBackgroundRotation({toRadians(rotationValues[0]), toRadians(rotationValues[1]), toRadians(rotationValues[2])});
    }
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    if (_imageNeedsDownload && _source) {
        _sphereTextureAddedToScene = NO;
        if (_source) {
            if (self.isHdr){
                [_hdrLoader loadHdrImage:_source];
            } else if([_assetLoader canLoadImageURL:_source.request.URL]) {
                [_assetLoader loadImage:_source];
            } else {
                [_imageAsyncLoader loadImage:_source];
            }
        }
        _imageNeedsDownload = NO;
    }
}

- (void)updateSceneWithSphereTexture {
    if(!_sphereTextureAddedToScene && _sphereTexture && self.parentHasAppeared) {
        self.node->getParentPortal()->setBackgroundSphere(_sphereTexture);
        float rotationValues[3] = {0.0f, 0.0f, 0.0f};
        populateFloatArrayFromNSArray(_rotation, rotationValues, 3);
        self.node->getParentPortal()->setBackgroundRotation({toRadians(rotationValues[0]), toRadians(rotationValues[1]), toRadians(rotationValues[2])});
        
        _sphereTextureAddedToScene = YES;
    }
}

- (void)parentDidAppear {
    [super parentDidAppear];
    //if the image loading is before the parent is added to the tree, then set the image.
    [self updateSceneWithSphereTexture];
}

#pragma mark - VRTAsyncLoaderEventDelegate

- (void)imageLoaderDidStart:(VRTImageAsyncLoader *)loader {
    if(self.onLoadStartViro) {
        self.onLoadStartViro(nil);
    }
}

- (void)imageLoaderDidEnd:(VRTImageAsyncLoader *)loader success:(BOOL)success image:(UIImage *)image {
    dispatch_async(dispatch_get_main_queue(), ^{
        if(success && image) {
            VROStereoMode mode = VROStereoMode::None;
            if (self.stereoMode){
                mode = VROTextureUtil::getStereoModeForString(std::string([self.stereoMode UTF8String]));
            }
            
            _sphereTexture = std::make_shared<VROTexture>(true,
                                                          VROMipmapMode::None, // Don't mipmap 360 images, wastes memory
                                                          std::make_shared<VROImageiOS>(image, self.format),
                                                          mode);
            [self updateSceneWithSphereTexture];
        }
        
        if(self.onLoadEndViro) {
            self.onLoadEndViro(@{@"success":@(success)});
        }
        if ((!success || !image) && self.onErrorViro) {
            self.onErrorViro(@{ @"error": @"Image failed to load" });
        }
    });
}

- (void)hdrImageLoaderDidStart:(VRTHDRImageAsyncLoader *)loader {
    if(self.onLoadStartViro) {
        self.onLoadStartViro(nil);
    }
}

- (void)hdrImageLoaderDidEnd:(VRTHDRImageAsyncLoader *)loader hdrTexture:(std::shared_ptr<VROTexture>)texture {
    dispatch_async(dispatch_get_main_queue(), ^{
        BOOL success = texture != nullptr;
        if(success) {
            _sphereTexture = texture;
            [self updateSceneWithSphereTexture];
        }

        if(self.onLoadEndViro) {
            self.onLoadEndViro(@{@"success":@(success)});
        }
        if ((!success) && self.onErrorViro) {
            self.onErrorViro(@{ @"error": @"Image failed to load" });
        }
    });
}

@end
