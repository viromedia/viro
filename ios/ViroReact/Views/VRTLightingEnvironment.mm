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
                [_imageAsyncLoader cancel];
                [_imageAsyncLoader loadImage:_source];
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
    if (![self shouldAppear]) {
        _targetedPortal->setLightingEnvironment(nullptr);
    }
    [super handleAppearanceChange];
}

#pragma mark - VRTAsyncLoaderEventDelegate
- (void)imageLoaderDidStart:(VRTImageAsyncLoader *)loader {
    if(self.onLoadStartViro) {
        self.onLoadStartViro(nil);
    }
}

- (void)imageLoaderDidEnd:(VRTImageAsyncLoader *)loader success:(BOOL)success image:(UIImage *)image {
    // Persist the in-memory UIImage data onto a temporary file to be processed by renderer.
    NSString *filePath = [self saveImageTempFile:image withName:@"temp_ibl_image.hdr"];

    // Process and generate a radiance hdr texture from the downloaded image file.
    std::string filePathStr = std::string([filePath UTF8String]);
    _environmentalLightTexture = VROHDRLoader::loadRadianceHDRTexture(filePathStr);
    [[NSFileManager defaultManager] removeItemAtPath:filePath error: nil];
    if (_environmentalLightTexture == nullptr){
        success = false;
    }
    
    // Update the lighting environment with the radiance hdr texture, if possible, and trigger callbacks.
    dispatch_async(dispatch_get_main_queue(), ^{
        if(success && image) {
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

// Saves the image data in memory to a temporary file with the given file name
- (NSString *)saveImageTempFile:(UIImage *)image withName:(NSString *)name {
    NSData *hdrData = UIImagePNGRepresentation(image);
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsPath = [paths objectAtIndex:0];
    NSString *filePath = [documentsPath stringByAppendingPathComponent:name];
    [hdrData writeToFile:filePath atomically:YES];
    return filePath;
}

@end
