//
//  VRT360Image.m
//  React
//
//  Created by Vik Advani on 3/3/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VROUtils.h"
#import "VRT360Image.h"
#import "RCTConvert.h"
#import "RCTUtils.h"
#import "VRTImageAsyncLoader.h"

@interface RCTImageSource (Viro)

@property (nonatomic, assign) BOOL packagerAsset;

@end

@implementation VRT360Image {
  std::shared_ptr<VROTexture> _sphereTexture;
  BOOL _sphereTextureAddedToScene;
  VRTImageAsyncLoader *_imageAsyncLoader;
}

@synthesize onLoadStartViro = _onLoadStartViro;
@synthesize onLoadEndViro = _onLoadEndViro;
@synthesize source = _source;
@synthesize rotation = _rotation;


- (instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super initWithBridge:bridge];
  if (self) {
    _sphereTextureAddedToScene = NO;
    _imageAsyncLoader = [[VRTImageAsyncLoader alloc] initWithDelegate:self];
  }
  
  return self;
}

// Loading from disk and network should both be async. Currently disk loading will block
// Reference RCTImage package for how to do image loaders properly.
- (void)setSource:(RCTImageSource *)source {
  _source = source;
  [self loadImageWhenReady];

}

- (void)setOnLoadStartViro:(RCTDirectEventBlock)onLoadStart {
  _onLoadStartViro = onLoadStart;
  [self loadImageWhenReady];
}

- (void)setOnLoadEndViro:(RCTDirectEventBlock)onLoadEnd {
  _onLoadEndViro = onLoadEnd;
  [self loadImageWhenReady];
}

- (void)setRotation:(NSArray<NSNumber *> *)rotation {
  _rotation = [rotation copy];
  if (_sphereTextureAddedToScene) {
    float rotationValues[3] = {0.0f, 0.0f, 0.0f};
    populateFloatArrayFromNSArray(_rotation, rotationValues, 3);
    self.scene->setBackgroundRotation({toRadians(rotationValues[0]), toRadians(rotationValues[1]), toRadians(rotationValues[2])});
  }
}

- (void)loadImageWhenReady {
  if (self.source){
    [self loadImage:self.source];
  }
}

-(void)loadImage:(RCTImageSource *)imageSource {
  _sphereTextureAddedToScene = NO;
  [_imageAsyncLoader loadImage:imageSource];
}

-(void)updateSceneWithSphereTexture{
  if(!_sphereTextureAddedToScene && _sphereTexture && self.scene) {
    self.scene->setBackgroundSphere(_sphereTexture);
    float rotationValues[3] = {0.0f, 0.0f, 0.0f};
    populateFloatArrayFromNSArray(_rotation, rotationValues, 3);
    self.scene->setBackgroundRotation({toRadians(rotationValues[0]), toRadians(rotationValues[1]), toRadians(rotationValues[2])});

    _sphereTextureAddedToScene = YES;
  }
}

- (void)viewWillAppear {
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
    _sphereTexture = std::make_shared<VROTexture>(std::make_shared<VROImageiOS>(image));
    [self updateSceneWithSphereTexture];
    if(self.onLoadEndViro) {
      self.onLoadEndViro(@{@"success":@(success)});
    }
  });
}

@end
