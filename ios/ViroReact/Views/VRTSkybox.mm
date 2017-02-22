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
@property (readwrite, nonatomic) BOOL cubeTextureAddedToScene;
@property (readwrite, nonatomic) NSMutableDictionary<NSString *, VRTImageAsyncLoader *> *imageAsyncLoaders;
@property (readwrite, nonatomic) NSMutableDictionary<NSString *, UIImage *> *downloadedImages;

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
  }
  
  return self;
}

- (void)setColor:(UIColor *)color {
  _color = color;
  [self updateSceneWithSkybox];
}

- (void)setSource:(VRTCubeMap *)source {
  _source = source;
  [self loadImageWhenReady];
}

- (void)setOnLoadStart:(RCTDirectEventBlock)onLoadStart {
  _onViroSkyBoxLoadStart = onLoadStart;
  [self loadImageWhenReady];
}

- (void)setOnLoadEnd:(RCTDirectEventBlock)onLoadEnd {
  _onViroSkyBoxLoadEnd = onLoadEnd;
  [self loadImageWhenReady];
}

- (void)loadImageWhenReady {
  if (self.source && !self.color) {
    self.cubeTextureAddedToScene = false;
    [self.downloadedImages removeAllObjects];
    
    for (NSString *key in [VRTCubeMap keys]) {
      VRTImageAsyncLoader *loader = [[VRTImageAsyncLoader alloc] initWithDelegate:self];
      loader.tag = key;
      
      [self.imageAsyncLoaders setObject:loader forKey:key];
      RCTImageSource *imageSource = [self.source imageSourceForKey:key];
      [loader loadImage:imageSource];
    }
  }
}

-(void)updateSceneWithSkybox {
  if(!_cubeTextureAddedToScene && self.scene) {
    if (_cubeTexture) {
      self.scene->setBackgroundCube(_cubeTexture);
      _cubeTextureAddedToScene = YES;
    }
    else if (_color) {
      CGFloat r, g, b, a;
      [_color getRed:&r green:&g blue:&b alpha:&a];
      
      VROVector4f v(r, g, b, a);
      self.scene->setBackgroundCube(v);
      _cubeTextureAddedToScene = YES;
    }
  }
}

- (void)viewWillAppear {
  //if the image loading is before the scene is set, then set the image.
  [self updateSceneWithSkybox];
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
    
    if (self.downloadedImages.count == 6) {
      UIImage *px = [self.downloadedImages objectForKey:@"px"];
      UIImage *nx = [self.downloadedImages objectForKey:@"nx"];
      UIImage *py = [self.downloadedImages objectForKey:@"py"];
      UIImage *ny = [self.downloadedImages objectForKey:@"ny"];
      UIImage *pz = [self.downloadedImages objectForKey:@"pz"];
      UIImage *nz = [self.downloadedImages objectForKey:@"nz"];
      
      if (px != nil && nx != nil && py != nil && ny != nil && pz != nil && nz != nil) {
        std::vector<std::shared_ptr<VROImage>> cubeImages =  {
            std::make_shared<VROImageiOS>(px),
            std::make_shared<VROImageiOS>(nx),
            std::make_shared<VROImageiOS>(py),
            std::make_shared<VROImageiOS>(ny),
            std::make_shared<VROImageiOS>(pz),
            std::make_shared<VROImageiOS>(nz)
        };
        
          _cubeTexture = std::make_shared<VROTexture>(VROTextureInternalFormat::RGBA8, cubeImages);
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
