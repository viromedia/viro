//
//  VRTImage.m
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

#import <React/RCTLog.h>
#import "VRTImage.h"
#import "VRTMaterialManager.h"
#import "VRTImageAsyncLoader.h"
#import "VRTPhotoLibraryAsyncLoader.h"

static float const kDefaultWidth = 1;
static float const kDefaultHeight = 1;

@implementation VRTImage {
    VRTImageAsyncLoader *_loader;
    VRTPhotoLibraryAsyncLoader *_assetLoader;
    BOOL _widthOrHeightPropSet;
    BOOL _widthOrHeightChanged;
    BOOL _geometryNeedsUpdate;
    BOOL _resizeModePropSet;
    float _scaledHeight;
    float _scaledWidth;
    float _downloadedImageWidth;
    float _downloadedImageHeight;
    float _u0;
    float _v0;
    float _u1;
    float _v1;
}

-(instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _loader = [[VRTImageAsyncLoader alloc] initWithDelegate:self];
        _assetLoader = [[VRTPhotoLibraryAsyncLoader alloc] initWithDelegate:self];
        _width = kDefaultWidth;
        _height = kDefaultHeight;
        _scaledWidth = kDefaultWidth;
        _scaledHeight = kDefaultHeight;
        _widthOrHeightPropSet = NO;
        _widthOrHeightChanged = YES;
        _resizeModePropSet = NO;
        _mipmap = YES;
        _imageNeedsDownload = NO;
        _format = VROTextureInternalFormat::RGBA8;
        _resizeMode = VROImageResizeMode::StretchToFill;
        _imageClipMode = VROImageClipMode::ClipToBounds; // Default to cropping if image overshoots with scaleToFill. Developers can override by passing None.
        _u0 = 0;
        _v0 = 0;
        _u1 = 1;
        _v1 = 1;
    }
    
    return self;
}

- (void)setPlaceholderSource:(VRTUIImageWrapper *)placeholderSource {
    _placeholderSource = placeholderSource;
}

- (void)setSource:(RCTImageSource *)source {
    _source = source;
    _imageNeedsDownload = YES;
}

- (void)setFormat:(VROTextureInternalFormat)format {
    _format = format;
    _imageNeedsDownload = YES;
}

- (void)setWidth:(float)width {
    _width = width;
    _scaledWidth = width;
    _widthOrHeightPropSet = YES;
    _widthOrHeightChanged = YES;
}

- (void)setHeight:(float)height {
    _height = height;
    _scaledHeight = height;
    _widthOrHeightPropSet = YES;
    _widthOrHeightChanged = YES;
}

- (void)setResizeMode:(VROImageResizeMode)resizeMode {
    _resizeMode = resizeMode;
    _resizeModePropSet = YES;
    _geometryNeedsUpdate = YES;
}

- (void)setImageClipMode:(VROImageClipMode)imageClipMode {
    _imageClipMode = imageClipMode;
    _geometryNeedsUpdate = YES;
}

- (void)updateSurface {
    float imageSurfaceWidth;
    float imageSurfaceHeight;
    if (_imageClipMode == VROImageClipMode::ClipToBounds && _resizeMode == VROImageResizeMode::ScaleToFill) {
        imageSurfaceWidth = _width;
        imageSurfaceHeight = _height;
    } else {
        imageSurfaceWidth = _scaledWidth;
        imageSurfaceHeight = _scaledHeight;
    }
    std::shared_ptr<VROSurface> imageSurface = VROSurface::createSurface(imageSurfaceWidth, imageSurfaceHeight, _u0, _v0, _u1, _v1);
    
    self.node->setGeometry(imageSurface);
    
    [self applyMaterials];
}

// Override applyMaterials so we can set our image in front
- (void)applyMaterials {
    [super applyMaterials];
    
    if (_texture && self.node->getGeometry()) {
        self.node->getGeometry()->getMaterials().front()->getDiffuse().setTexture(_texture);
        self.node->getGeometry()->getMaterials().front()->setTransparency(1.0);
    }
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    [self updatePlaceholderProps:changedProps];

    if (_imageNeedsDownload) {
        // Start loading the image
        if (_source) {
            if([_assetLoader canLoadImageURL:_source.request.URL]) {
                [_assetLoader loadImage:_source];
            } else {
                [_loader loadImage:_source];
            }
        }
    }

    _imageNeedsDownload = NO;
}

- (void)updatePlaceholderProps:(NSArray<NSString *> *)changedProps{
    if (_widthOrHeightChanged || _geometryNeedsUpdate) {
        [self resizeImageDimensions];
        [self updateSurface];
        _widthOrHeightChanged = NO;
        _geometryNeedsUpdate = NO;
    }
    
    if (_imageNeedsDownload) {
        // Set the placeholder while the image loads. If there is no placeholder, set the
        // diffuse color to transparent
        if (_source && !_texture && self.node && self.node->getGeometry()) {
            if (_placeholderSource) {
                std::shared_ptr<VROTexture> placeholderTexture = std::make_shared<VROTexture>(true,
                                                                                              VROMipmapMode::Runtime,
                                                                                              std::make_shared<VROImageiOS>(_placeholderSource.image,
                                                                                                                            VROTextureInternalFormat::RGBA8));
                self.node->getGeometry()->getMaterials().front()->getDiffuse().setTexture(placeholderTexture);
                self.node->getGeometry()->getMaterials().front()->setTransparency(1.0);
            }
            else {
                self.node->getGeometry()->getMaterials().front()->setTransparency(0.0);
            }
        }
    }
}

#pragma mark - VRTAsyncLoaderEventDelegate

- (void)imageLoaderDidStart:(VRTImageAsyncLoader *)loader {
    if(self.onLoadStartViro) {
        self.onLoadStartViro(nil);
    }
}

- (void)imageLoaderDidEnd:(VRTImageAsyncLoader *)loader success:(BOOL)success image:(UIImage *)image {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (success && image) {
            VROStereoMode mode = VROStereoMode::None;
            if (self.stereoMode){
                mode = VROTextureUtil::getStereoModeForString(std::string([self.stereoMode UTF8String]));
            }
            
            _texture = std::make_shared<VROTexture>(true,
                                                    self.mipmap ? VROMipmapMode::Runtime : VROMipmapMode::None,
                                                    std::make_shared<VROImageiOS>(image, self.format),
                                                    mode);
            [self updateMainImage:image.size.width height:image.size.height];
            [self applyMaterials];
        }
        
        if(self.onLoadEndViro) {
            self.onLoadEndViro(@{@"success":@(success)});
        }
        if ((!success || !image) && self.onErrorViro) {
            self.onErrorViro(@{ @"error": @"Image failed to load" });
        }
    });
}

- (void)updateMainImage:(float)width height:(float)height {
    _downloadedImageWidth = width;
    _downloadedImageHeight = height;
    
    // Check if width and height were set as props. If not, recreate the surface using
    // the aspect ratio of image.
    if (!_widthOrHeightPropSet){
        float ratio = width / height;
        _height = _width / ratio;
        _widthOrHeightChanged = NO;
        [self updateSurface];
    } else if (_resizeModePropSet) {
        // If width and height were set as props, along with resizeMode, we'll calculate scaled width & height of the image
        [self resizeImageDimensions];
        [self updateSurface];
    }
}

- (void)resizeImageDimensions {
    if(!_widthOrHeightPropSet || !_resizeModePropSet) {
        return;
    }
    float aspectRatio = _downloadedImageWidth / _downloadedImageHeight;
    float targetAspectRatio = _width / _height;
    
    switch(_resizeMode) {
        case VROImageResizeMode::ScaleToFit:
            if (targetAspectRatio <= aspectRatio) { // target is taller than content
                _scaledWidth = _width;
                _scaledHeight = _scaledWidth / aspectRatio;
            } else { // target is wider than content
                _scaledHeight = _height;
                _scaledWidth = _scaledHeight * aspectRatio;
            };
            break;
        case VROImageResizeMode::ScaleToFill:
            if (targetAspectRatio <= aspectRatio) { // target is taller than content
                _scaledHeight = _height;
                _scaledWidth = _scaledHeight * aspectRatio;
            } else { // target is wider than content
                _scaledWidth = _width;
                _scaledHeight = _scaledWidth / aspectRatio;
            };
            // If clipMode is set to clipToBounds, we need to calculate u,v values so that the image clips/crops to the image view bounds
            if (_imageClipMode == VROImageClipMode::ClipToBounds) {
                float clipWidth = fabsf(_scaledWidth - _width) / _scaledWidth;
                float clipHeight = fabsf(_scaledHeight - _height)/ _scaledHeight;
                _u0 = clipWidth / 2;
                _v0 = clipHeight / 2;
                _u1 = 1 - clipWidth / 2;
                _v1 = 1 - clipHeight / 2;
            }
            break;
        case VROImageResizeMode::StretchToFill:
            _scaledWidth = _width;
            _scaledHeight = _height;
            _u0 = 0;
            _v0 = 0;
            _u1 = 1;
            _v1 = 1;
    }
}

@end

@implementation RCTConvert (VRTImage)

+ (VROTextureInternalFormat)VROTextureInternalFormat:(id)json {
    if (![json isKindOfClass:[NSString class]]) {
        return VROTextureInternalFormat::RGBA8;
    }
    
    NSString *value = (NSString *)json;
    if([value caseInsensitiveCompare:@"RGBA8"] == NSOrderedSame ) {
        return VROTextureInternalFormat::RGBA8;
    } else if([value caseInsensitiveCompare:@"RGBA4"] == NSOrderedSame) {
        return VROTextureInternalFormat::RGBA4;
    } else if([value caseInsensitiveCompare:@"RGB565"] == NSOrderedSame) {
        return VROTextureInternalFormat::RGB565;
    }
    
    return VROTextureInternalFormat::RGBA8;;
}

+ (VROImageResizeMode)VROImageResizeMode:(id)json {
    if (![json isKindOfClass:[NSString class]]) {
        RCTLogError(@"Error setting string. String required, received: %@", json);
        return VROImageResizeMode::StretchToFill;
    }
    
    NSString *value = (NSString *)json;
    if ([value caseInsensitiveCompare:@"ScaleToFill"] == NSOrderedSame) {
        return VROImageResizeMode::ScaleToFill;
    } else if ([value caseInsensitiveCompare:@"ScaleToFit"] == NSOrderedSame) {
        return VROImageResizeMode::ScaleToFit;
    } else if ([value caseInsensitiveCompare:@"StretchToFill"] == NSOrderedSame) {
        return VROImageResizeMode::StretchToFill;
    }
    
    return VROImageResizeMode::StretchToFill;
}

+(VROImageClipMode)VROImageClipMode:(id)json {
    if (![json isKindOfClass:[NSString class]]) {
        RCTLogError(@"Error setting string. String required, received: %@", json);
        return VROImageClipMode::None;
    }
    
    NSString *value = (NSString *)json;
    
    if ([value caseInsensitiveCompare:@"None"] == NSOrderedSame) {
        return VROImageClipMode::None;
    }
    else if ([value caseInsensitiveCompare:@"ClipToBounds"] == NSOrderedSame) {
        return VROImageClipMode::ClipToBounds;
    }
    return VROImageClipMode::None;
}

@end

