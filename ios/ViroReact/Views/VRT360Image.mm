//
//  VRT360Image.m
//  React
//
//  Created by Vik Advani on 3/3/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <React/RCTConvert.h>
#import <React/RCTUtils.h>
#import "VRTUtils.h"
#import "VRT360Image.h"
#import "VRTImageAsyncLoader.h"

@interface RCTImageSource (Viro)

@property (nonatomic, assign) BOOL packagerAsset;

@end

@implementation VRT360Image {
  std::shared_ptr<VROTexture> _sphereTexture;
  BOOL _sphereTextureAddedToScene;
  BOOL _imageNeedsDownload;
  VRTImageAsyncLoader *_imageAsyncLoader;
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
    self.scene->setBackgroundRotation({toRadians(rotationValues[0]), toRadians(rotationValues[1]), toRadians(rotationValues[2])});
  }
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
  if (_imageNeedsDownload && _source) {
    _sphereTextureAddedToScene = NO;
    [_imageAsyncLoader loadImage:_source];
    
    _imageNeedsDownload = NO;
  }
}

- (void)updateSceneWithSphereTexture {
  if(!_sphereTextureAddedToScene && _sphereTexture && self.scene) {
    self.scene->setBackgroundSphere(_sphereTexture);
    float rotationValues[3] = {0.0f, 0.0f, 0.0f};
    populateFloatArrayFromNSArray(_rotation, rotationValues, 3);
    self.scene->setBackgroundRotation({toRadians(rotationValues[0]), toRadians(rotationValues[1]), toRadians(rotationValues[2])});

    _sphereTextureAddedToScene = YES;
  }
}

- (void)sceneWillAppear {
  //if the image loading is before the scene is set, then set the image.
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
        
      _sphereTexture = std::make_shared<VROTexture>(self.format,
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

@end
