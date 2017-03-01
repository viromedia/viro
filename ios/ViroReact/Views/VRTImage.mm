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
  std::shared_ptr<VROSurface> _surface;
  VRTImageAsyncLoader *_loader;
  std::shared_ptr<VROTexture> _texture;
  BOOL _widthOrHeightPropSet;
  BOOL _sourceChanged;
}

-(instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super initWithBridge:bridge];
  if (self) {
    _loader = [[VRTImageAsyncLoader alloc] initWithDelegate:self];
    _width = kDefaultWidth;
    _height = kDefaultHeight;
    _widthOrHeightPropSet = NO;
    _mipmap = YES;
    _sourceChanged = NO;
    _format = VROTextureInternalFormat::RGBA8;
  }
  
  return self;
}

- (void)setPlaceHolderSource:(UIImage *)placeholderSource {
  _placeholderSource = placeholderSource;
}

- (void)setSource:(RCTImageSource *)source {
  _source = source;
  _sourceChanged = YES;
}

- (void)setWidth:(float)width {
  _width = width;
  _widthOrHeightPropSet = YES;
}

- (void)setHeight:(float)height {
  _height = height;
  _widthOrHeightPropSet = YES;
}

- (void)updateSurface {
  _surface = VROSurface::createSurface(_width, _height);
  [self node]->setGeometry(_surface);
  [self applyMaterials];
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
  [self updateSurface];

  if (_sourceChanged) {
    // Set the placeholder while the image loads
    if (_placeholderSource && _source) {
      std::shared_ptr<VROTexture> placeholderTexture = std::make_shared<VROTexture>(VROTextureInternalFormat::RGBA8,
                                                                                    VROMipmapMode::Runtime,
                                                                                    std::make_shared<VROImageiOS>(_placeholderSource,
                                                                                                                  VROTextureInternalFormat::RGBA8));
        _surface->getMaterials().front()->getDiffuse().setTexture(placeholderTexture);
    }
    
    // Start loading the image
    if (_source) {
      [_loader loadImage:_source];
    }
    _sourceChanged = NO;
  }
  else if (_texture) {
    _surface->getMaterials().front()->getDiffuse().setTexture(_texture);
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
    if (success && image!=nil) {
        _texture = std::make_shared<VROTexture>(self.format,
                                                self.mipmap ? VROMipmapMode::Runtime : VROMipmapMode::None,
                                                std::make_shared<VROImageiOS>(image, self.format));
        
      // Check if width and height were set as props. If not, recreate the surface using
      // the aspect ratio of image.
      if (!_widthOrHeightPropSet){
        float ratio = image.size.width / image.size.height;
        _height = _width / ratio;
        [self updateSurface];
      }
        
      _surface->getMaterials().front()->getDiffuse().setTexture(_texture);
    }

    if(self.onLoadEndViro) {
      self.onLoadEndViro(@{@"success":@(success)});
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

