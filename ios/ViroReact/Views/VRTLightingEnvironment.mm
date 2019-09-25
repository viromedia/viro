//
//  VRTLightingEnvironment.m
//  React
//
//  Copyright © 2018 Viro Media. All rights reserved.
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
#import "VRTLightingEnvironment.h"

@interface RCTImageSource (Viro)
@property (nonatomic, assign) BOOL packagerAsset;
@end

@implementation VRTLightingEnvironment {
    std::shared_ptr<VROTexture> _environmentalLightTexture;
    BOOL _enviromentalLightTextureApplied;
    BOOL _textureNeedsDownload;
    VRTHDRImageAsyncLoader *_imageHdrAsyncLoader;
    std::shared_ptr<VROPortal> _targetedPortal;
}

@synthesize onLoadStartViro = _onLoadStartViro;
@synthesize onLoadEndViro = _onLoadEndViro;
@synthesize source = _source;

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _targetedPortal = nullptr;
        _enviromentalLightTextureApplied = NO;
        _textureNeedsDownload = NO;
        _imageHdrAsyncLoader = [[VRTHDRImageAsyncLoader alloc] initWithDelegate:self];
    }

    return self;
}

// Loading from disk and network should both be async. Currently disk loading will block
// Reference RCTImage package for how to do image loaders properly.
- (void)setSource:(RCTImageSource *)source {
    _source = source;
    _textureNeedsDownload = YES;
}

- (void)setOnLoadStartViro:(RCTDirectEventBlock)onLoadStart {
    _onLoadStartViro = onLoadStart;
}

- (void)setOnLoadEndViro:(RCTDirectEventBlock)onLoadEnd {
    _onLoadEndViro = onLoadEnd;
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    if (_textureNeedsDownload && _source) {
        _enviromentalLightTextureApplied = NO;
        if (_source && [_imageHdrAsyncLoader isHdrSource:_source]) {
            [_imageHdrAsyncLoader cancel];
            [_imageHdrAsyncLoader loadHdrImage:_source];
        } else {
            if (self.onErrorViro) {
                self.onErrorViro(@{ @"error": @"Lighting Environment Image source is not of type hdr!" });
            }
        }
        _textureNeedsDownload = NO;
    }
}

- (void)updateSceneLightingEnvironment {
    if(!_enviromentalLightTextureApplied && _environmentalLightTexture && self.parentHasAppeared) {
        _targetedPortal = self.node->getParentPortal();
        _targetedPortal->setLightingEnvironment(_environmentalLightTexture);
        _enviromentalLightTextureApplied = YES;
    }
}

- (void)parentDidAppear {
    [super parentDidAppear];
    //if the image loading is before the parent is added to the tree, then set the image.
    [self updateSceneLightingEnvironment];
}

- (void)handleAppearanceChange {
    if (![self shouldAppear] && _targetedPortal) {
        _targetedPortal->setLightingEnvironment(nullptr);
    }
    [super handleAppearanceChange];
}

#pragma mark - VRTAsyncLoaderEventDelegate
- (void)hdrImageLoaderDidStart:(VRTHDRImageAsyncLoader *)loader {
    if(self.onLoadStartViro) {
        self.onLoadStartViro(nil);
    }
}

- (void)hdrImageLoaderDidEnd:(VRTHDRImageAsyncLoader *)loader hdrTexture:(std::shared_ptr<VROTexture>)texture {
    // Update the lighting environment with the radiance hdr texture, if possible, and trigger callbacks.
    dispatch_async(dispatch_get_main_queue(), ^{
        _environmentalLightTexture = texture;
        BOOL success = texture != nullptr;
        if(success) {
            [self updateSceneLightingEnvironment];
        }
        
        if(self.onLoadEndViro) {
            self.onLoadEndViro(@{@"success":@(success)});
        }
        
        if (!success && self.onErrorViro) {
            self.onErrorViro(@{ @"error": @"Image failed to load" });
        }
    });
}
@end
