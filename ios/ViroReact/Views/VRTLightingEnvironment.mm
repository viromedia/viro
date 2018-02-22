//
//  VRTLightingEnvironment.m
//  React
//
//  Copyright © 2018 Viro Media. All rights reserved.
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
