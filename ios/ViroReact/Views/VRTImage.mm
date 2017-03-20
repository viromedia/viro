//
//  VRTSurface.m
//  React
//
//  Created by Vik Advani on 3/3/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <React/RCTLog.h>
#import "VRTImage.h"
#import "VRTMaterialManager.h"
#import "VRTImageAsyncLoader.h"

static float const kDefaultWidth = 1;
static float const kDefaultHeight = 1;

@implementation VRTImage {
  VRTImageAsyncLoader *_loader;
  std::shared_ptr<VROTexture> _texture;
  BOOL _widthOrHeightPropSet;
  BOOL _widthOrHeightChanged;
  BOOL _imageNeedsDownload;
}

-(instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super initWithBridge:bridge];
  if (self) {
    _loader = [[VRTImageAsyncLoader alloc] initWithDelegate:self];
    _width = kDefaultWidth;
    _height = kDefaultHeight;
    _widthOrHeightPropSet = NO;
    _widthOrHeightChanged = YES;
    _mipmap = YES;
    _imageNeedsDownload = NO;
    _format = VROTextureInternalFormat::RGBA8;
  }
  
  return self;
}

- (void)setPlaceHolderSource:(VRTUIImageWrapper *)placeholderSource {
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
  _widthOrHeightPropSet = YES;
  _widthOrHeightChanged = YES;
}

- (void)setHeight:(float)height {
  _height = height;
  _widthOrHeightPropSet = YES;
  _widthOrHeightChanged = YES;
}

- (void)updateSurface {
  std::shared_ptr<VROSurface> surface = VROSurface::createSurface(_width, _height);

  self.node->setGeometry(surface);
  [self applyMaterials];
}

// Override applyMaterials so we can set our image in front
- (void)applyMaterials {
  [super applyMaterials];
  
  if (_texture && self.node->getGeometry()) {
    self.node->getGeometry()->getMaterials().front()->getDiffuse().setTexture(_texture);
  }
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
  if (_widthOrHeightChanged) {
    [self updateSurface];
    _widthOrHeightChanged = NO;
  }

  if (_imageNeedsDownload) {
    // Set the placeholder while the image loads
    if (_placeholderSource.image && _source && !_texture && self.node && self.node->getGeometry()) {
      std::shared_ptr<VROTexture> placeholderTexture = std::make_shared<VROTexture>(VROTextureInternalFormat::RGBA8,
                                                                                    VROMipmapMode::Runtime,
                                                                                    std::make_shared<VROImageiOS>(_placeholderSource.image,
                                                                                                                  VROTextureInternalFormat::RGBA8));
        self.node->getGeometry()->getMaterials().front()->getDiffuse().setTexture(placeholderTexture);
    }
    
    // Start loading the image
    if (_source) {
      [_loader loadImage:_source];
    }
    _imageNeedsDownload = NO;
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
      _texture = std::make_shared<VROTexture>(self.format,
                                              self.mipmap ? VROMipmapMode::Runtime : VROMipmapMode::None,
                                              std::make_shared<VROImageiOS>(image, self.format));
        
      // Check if width and height were set as props. If not, recreate the surface using
      // the aspect ratio of image.
      if (!_widthOrHeightPropSet){
        float ratio = image.size.width / image.size.height;
        _height = _width / ratio;
        [self updateSurface];
        
        _widthOrHeightChanged = NO;
      }
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

@end

