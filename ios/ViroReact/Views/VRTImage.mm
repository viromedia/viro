//
//  VRTSurface.m
//  React
//
//  Created by Vik Advani on 3/3/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTImage.h"
#import "VRTLabel.h"
#import "RCTLog.h"
#import "VROMaterialManager.h"
#import "VRTImageAsyncLoader.h"

static float const kDefaultWidth = 1;
static float const kDefaultHeight = 1;

@implementation VRTImage {
  std::shared_ptr<VROSurface> _surface;
  VRTImageAsyncLoader *_loader;
  BOOL _imageAddedToScene;
  BOOL _materialAddedToScene;
  BOOL _widthOrHeightPropSet;
  std::shared_ptr<VROTexture> _texture;
  std::shared_ptr<VROMaterial> _defaultMaterial;
}

@synthesize onGaze = _onGaze;
@synthesize onTap = _onTap;
@synthesize delegate = _delegate;

-(instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super initWithBridge:bridge];
  if (self) {
    _imageAddedToScene = NO;
    _materialAddedToScene = NO;
    _widthOrHeightPropSet = NO;
    _loader = [[VRTImageAsyncLoader alloc] initWithDelegate:self];
    _width = kDefaultWidth;
    _height = kDefaultHeight;
    [self updateGeometry];
  }
  
  return self;
}

- (void)setMaterials:(NSArray<NSString *> *)materialArray {
  [super setMaterials:materialArray];

  if([materialArray count] >= 1) {
    VROMaterialManager *materialManager = [self.bridge materialManager];
    _defaultMaterial =  [materialManager getMaterialByName:materialArray[0]];
  }
  _materialAddedToScene = YES;
}

/* Set a new place holder image that will show if a material OR image set via source is not ALREADY shown */
- (void)setPlaceHolderSource:(UIImage *)placeHolderSource {
  _placeHolderSource = placeHolderSource;
  if(!_materialAddedToScene && !_imageAddedToScene) {
    std::shared_ptr<VROTexture> placeHolderTexture = std::make_shared<VROTexture>(std::make_shared<VROImageiOS>(placeHolderSource));
    std::shared_ptr<VROMaterial> placeHolderMaterial = std::make_shared<VROMaterial>();
    placeHolderMaterial->getDiffuse().setTexture(placeHolderTexture);
    _surface->getMaterials().clear();
    _surface->getMaterials().push_back(placeHolderMaterial);
  }
}


// Loading from disk and network should both be async. Currently disk loading will block
// Reference RCTImage package for how to do image loaders properly.
- (void)setSource:(RCTImageSource *)source {
  _source = source;
  [self loadImageWhenReady];
  
}

-(void)setWidth:(float)width {
  _width = width;
  _widthOrHeightPropSet = YES;
  [self updateGeometry];

}

-(void)setHeight:(float)height {
  _height = height;
  _widthOrHeightPropSet = YES;
  [self updateGeometry];
}

- (void)setOnLoadStart:(RCTDirectEventBlock)onLoadStart {
  _onLoadStartViro = onLoadStart;
  [self loadImageWhenReady];
}

- (void)setOnLoadEnd:(RCTDirectEventBlock)onLoadEnd {
  _onLoadEndViro = onLoadEnd;
  [self loadImageWhenReady];
}

- (void)loadImageWhenReady {
  if (self.source){
    [self loadImage:self.source];
  } else {
    _imageAddedToScene = NO;
    if(_defaultMaterial) {
      [self applyMaterials];
    }
  }
}

-(void)loadImage:(RCTImageSource *)imageSource {
  _imageAddedToScene = NO;
  [_loader loadImage:imageSource];
}

-(void)updateImage {
  if(!_imageAddedToScene && _texture) {
    // Copy existing default material properties if default exists and change just the diffuse texture.
    std::shared_ptr<VROMaterial> newMaterial = _defaultMaterial ? std::make_shared<VROMaterial>(_defaultMaterial) : std::make_shared<VROMaterial>();
    newMaterial->getDiffuse().setTexture(_texture);
    _surface->getMaterials().clear();
    _surface->getMaterials().push_back(newMaterial);
    _imageAddedToScene = YES;
  }
}

-(void)updateGeometry {
  std::shared_ptr<VROMaterial> oldSurfaceMaterial;
  if(_surface) {
    oldSurfaceMaterial = _surface->getMaterials()[0];
  }
  
  _surface = VROSurface::createSurface(_width, _height);
  if(oldSurfaceMaterial) {
    _surface->getMaterials().clear();
    _surface->getMaterials().push_back(oldSurfaceMaterial);
  }

  [self node]->setGeometry(_surface);
}

#pragma mark - VRTAsyncLoaderEventDelegate

- (void)imageLoaderDidStart:(VRTImageAsyncLoader *)loader {
  if(self.onLoadStartViro) {
    self.onLoadStartViro(nil);
  }
}

- (void)imageLoaderDidEnd:(VRTImageAsyncLoader *)loader success:(BOOL)success image:(UIImage *)image {
  dispatch_async(dispatch_get_main_queue(), ^{
    if(success && image!=nil) {
      _texture = std::make_shared<VROTexture>(std::make_shared<VROImageiOS>(image));
      // Check if width and height were set as props, if not recreate surface using aspect ratio of image.
      if(!_widthOrHeightPropSet){
        float ratio = image.size.width/image.size.height;
        _height = _height/ratio;
        _surface = VROSurface::createSurface(_width, _height);
        [self updateImage];
        [self node]->setGeometry(_surface);
      }else{
        [self updateImage];
      }
    }

    if(self.onLoadEndViro) {
      self.onLoadEndViro(@{@"success":@(success)});
    }
  });
}

@end
