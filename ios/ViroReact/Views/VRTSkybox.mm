//
//  VRTSkybox.m
//  React
//
//  Created by Raj Advani on 10/5/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <React/RCTConvert.h>
#import <React/RCTUtils.h>
#import <React/RCTLog.h>
#import "VRTSkybox.h"
#import "VRTUtils.h"

#import "VRTImageAsyncLoader.h"

@interface VRTSkybox ()

@property (readwrite, nonatomic) std::shared_ptr<VROTexture> cubeTexture;
@property (readwrite, nonatomic) NSMutableDictionary<NSString *, VRTImageAsyncLoader *> *imageAsyncLoaders;
@property (readwrite, nonatomic) NSMutableDictionary<NSString *, UIImage *> *downloadedImages;
@property (readwrite, nonatomic) BOOL useImageForScene;
@property (readwrite, nonatomic) BOOL skyboxNeedsUpdate;

@end

@implementation VRTSkybox

@synthesize onViroSkyBoxLoadStart = _onViroSkyBoxLoadStart;
@synthesize onViroSkyBoxLoadEnd = _onViroSkyBoxLoadEnd;
@synthesize source = _source;

- (instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super initWithBridge:bridge];
  if (self) {
    self.imageAsyncLoaders = [[NSMutableDictionary alloc] init];
    self.downloadedImages = [[NSMutableDictionary alloc] init];
    self.format = VROTextureInternalFormat::RGBA8;
    self.useImageForScene = YES;
    self.skyboxNeedsUpdate = NO;
  }
  
  return self;
}

- (void)setColor:(UIColor *)color {
  _color = color;
  _useImageForScene = (color == nil);
  _skyboxNeedsUpdate = YES;
}

- (void)setSource:(VRTCubeMap *)source {
  _source = source;
  _useImageForScene = (source != nil);
  _skyboxNeedsUpdate = YES;
}

- (void)setOnLoadStart:(RCTDirectEventBlock)onLoadStart {
  _onViroSkyBoxLoadStart = onLoadStart;
}

- (void)setOnLoadEnd:(RCTDirectEventBlock)onLoadEnd {
  _onViroSkyBoxLoadEnd = onLoadEnd;
}

- (void)setFormat:(VROTextureInternalFormat)format {
  _format = format;
  _skyboxNeedsUpdate = YES;
}

- (void)loadImageAsync {
  [self.downloadedImages removeAllObjects];
  
  for (NSString *key in [VRTCubeMap keys]) {
    VRTImageAsyncLoader *loader = [[VRTImageAsyncLoader alloc] initWithDelegate:self];
    loader.tag = key;
    
    [self.imageAsyncLoaders setObject:loader forKey:key];
    RCTImageSource *imageSource = [self.source imageSourceForKey:key];
    [loader loadImage:imageSource];
  }
}

-(void)updateSceneWithSkybox {
  if(self.scene) {
    if (_useImageForScene && _cubeTexture) {
      self.scene->setBackgroundCube(_cubeTexture);
    }
    else if (_color) {
      CGFloat r, g, b, a;
      [_color getRed:&r green:&g blue:&b alpha:&a];
      
      VROVector4f v(r, g, b, a);
      self.scene->setBackgroundCube(v);
    }
  }
}

- (void)sceneWillAppear {
  //if the image loading is before the scene is set, then set the image.
  [self updateSceneWithSkybox];
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
  if (self.skyboxNeedsUpdate) {
    if (self.source && self.useImageForScene) {
      [self loadImageAsync];
    }
    else {
      [self updateSceneWithSkybox];
    }
    self.skyboxNeedsUpdate = NO;
  }
}

#pragma mark - VRTAsyncLoaderEventDelegate

- (void)imageLoaderDidStart:(VRTImageAsyncLoader *)loader {
  // Invoke these delegates only once (for the px image)
  if (loader.tag == @"px") {
    if(self.onViroSkyBoxLoadStart) {
      self.onViroSkyBoxLoadStart(nil);
    }
  }
}

- (void)imageLoaderDidEnd:(VRTImageAsyncLoader *)loader success:(BOOL)success image:(UIImage *)image {
  dispatch_async(dispatch_get_main_queue(), ^{
    [self.downloadedImages setValue:image forKey:loader.tag];
    //Check for image size, error in debug if the size is not power of two.
    if (success) {
      if (image.size.width != image.size.height) {
        RCTLogError(@"Skybox textures must be square, received image with dimensions %@",
                      NSStringFromCGSize(image.size));
      }
    }
    
    if (self.downloadedImages.count == 6) {
      UIImage *px = [self.downloadedImages objectForKey:@"px"];
      UIImage *nx = [self.downloadedImages objectForKey:@"nx"];
      UIImage *py = [self.downloadedImages objectForKey:@"py"];
      UIImage *ny = [self.downloadedImages objectForKey:@"ny"];
      UIImage *pz = [self.downloadedImages objectForKey:@"pz"];
      UIImage *nz = [self.downloadedImages objectForKey:@"nz"];
      
      if (px != nil && nx != nil && py != nil && ny != nil && pz != nil && nz != nil) {
        std::vector<std::shared_ptr<VROImage>> cubeImages =  {
            std::make_shared<VROImageiOS>(px, self.format),
            std::make_shared<VROImageiOS>(nx, self.format),
            std::make_shared<VROImageiOS>(py, self.format),
            std::make_shared<VROImageiOS>(ny, self.format),
            std::make_shared<VROImageiOS>(pz, self.format),
            std::make_shared<VROImageiOS>(nz, self.format)
        };
        
        _cubeTexture = std::make_shared<VROTexture>(self.format, cubeImages);
        [self updateSceneWithSkybox];
        
        [self.downloadedImages removeAllObjects];
      }
      
      if(self.onViroSkyBoxLoadEnd) {
        self.onViroSkyBoxLoadEnd(@{@"success":@(success)});
      }
    }
  });
}

@end
