//
//  VRTSurface.m
//  React
//
//  Created by Vik Advani on 3/3/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTImage.h"
#import "RCTLog.h"
#import "VROMaterialManager.h"
#import "VRTImageAsyncLoader.h"

static float const kDefaultWidth = 1;
static float const kDefaultHeight = 1;

@implementation VRTImage {
  std::shared_ptr<VROSurface> _surface;
  VRTImageAsyncLoader *_loader;
  std::shared_ptr<VROTexture> _texture;
  BOOL _widthOrHeightPropSet;
}

-(instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super initWithBridge:bridge];
  if (self) {
    _loader = [[VRTImageAsyncLoader alloc] initWithDelegate:self];
    _width = kDefaultWidth;
    _height = kDefaultHeight;
    _widthOrHeightPropSet = NO;
  }
  
  return self;
}

- (void)setPlaceHolderSource:(UIImage *)placeholderSource {
  _placeholderSource = placeholderSource;
}

- (void)setSource:(RCTImageSource *)source {
    _source = source;
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

    // Set the placeholder while the image loads
    if(_placeholderSource) {
        std::shared_ptr<VROTexture> placeholderTexture = std::make_shared<VROTexture>(std::make_shared<VROImageiOS>(_placeholderSource));
        _surface->getMaterials().front()->getDiffuse().setTexture(placeholderTexture);
    }
    
    // Start loading the image
    if (_source) {
        [_loader loadImage:_source];
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
      _texture = std::make_shared<VROTexture>(std::make_shared<VROImageiOS>(image));
        
      // Check if width and height were set as props. If not, recreate the surface using
      // the aspect ratio of image.
      if (!_widthOrHeightPropSet){
        float ratio = image.size.width / image.size.height;
        _height = _height / ratio;
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
