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
#import "VRTImageAsyncLoader.h"
#import "VRTPhotoLibraryAsyncLoader.h"

@interface RCTImageSource (Viro)
@property (nonatomic, assign) BOOL packagerAsset;
@end

@implementation VRTLightingEnvironment {
    std::shared_ptr<VROTexture> _environmentalLightTexture;
    BOOL _enviromentalLightTextureApplied;
    BOOL _textureNeedsDownload;
    VRTImageAsyncLoader *_imageAsyncLoader;
    VRTPhotoLibraryAsyncLoader *_assetLoader;
}

@synthesize onLoadStartViro = _onLoadStartViro;
@synthesize onLoadEndViro = _onLoadEndViro;
@synthesize source = _source;

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _enviromentalLightTextureApplied = NO;
        _textureNeedsDownload = NO;
        _imageAsyncLoader = [[VRTImageAsyncLoader alloc] initWithDelegate:self];
        _assetLoader = [[VRTPhotoLibraryAsyncLoader alloc] initWithDelegate:self];
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
        if (_source) {
            if([_assetLoader canLoadImageURL:_source.request.URL]) {
                [_assetLoader loadImage:_source];
            } else {
                [_imageAsyncLoader loadImage:_source];
            }
        }
        _textureNeedsDownload = NO;
    }
}

- (void)updateSceneLightingEnvironment {
    if(!_enviromentalLightTextureApplied && _environmentalLightTexture && self.parentHasAppeared) {
        self.node->getParentPortal()->setLightingEnvironment(_environmentalLightTexture);
        _enviromentalLightTextureApplied = YES;
    }
}

- (void)parentDidAppear {
    [super parentDidAppear];
    //if the image loading is before the parent is added to the tree, then set the image.
    [self updateSceneLightingEnvironment];
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
            _environmentalLightTexture = std::make_shared<VROTexture>(VROTextureInternalFormat::RGB9_E5,
                                                                      true,
                                                                      VROMipmapMode::None,
                                                                      std::make_shared<VROImageiOS>(image, VROTextureInternalFormat::RGB9_E5));
            [self updateSceneLightingEnvironment];
        }
        
        if(self.onLoadEndViro) {
            self.onLoadEndViro(@{@"success":@(success)});
        }
        if ((!success || !image) && self.onErrorViro) {
            self.onErrorViro(@{ @"error": @"Image failed to load" });
        }
    });
}

@end
