//
//  VRTButton.m
//  React
//
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTButton.h"
#import "VRTImageAsyncLoader.h"
 #import "VROMaterialManager.h"

@implementation VRTButton {
  VRTImageAsyncLoader *_defaultSourceLoader;
  VRTImageAsyncLoader *_gazeSourceLoader;
  VRTImageAsyncLoader *_tapSourceLoader;
  std::shared_ptr<VROTexture> _defaultTexture;
  std::shared_ptr<VROTexture> _gazeTexture;
  std::shared_ptr<VROTexture> _tapTexture;
  std::shared_ptr<VROMaterial> _defaultMaterial;
  std::shared_ptr<VROMaterial> _gazeMaterial;
  std::shared_ptr<VROMaterial> _tapMaterial;
}

float const BUTTON_PRE_BOUNCE_SCALE = 0.9f;
float const BUTTON_BOUNCE_DURATION_SEC = 0.5f;
double const ON_TAP_UP_CALLBACK_SEC = 0.1 * NSEC_PER_SEC;
NSString * const kGazeSource = @"gazeSource";
NSString * const kTapSource = @"tapSource";
NSString * const kDefaultSource = @"defaultSource";
@synthesize source = _source;

-(instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super initWithBridge:bridge];
  _defaultMaterial = nil;
  _gazeMaterial = nil;
  _tapMaterial = nil;
  _defaultSourceLoader = [[VRTImageAsyncLoader alloc] initWithDelegate:self];
  _gazeSourceLoader = [[VRTImageAsyncLoader alloc] initWithDelegate:self];
  _tapSourceLoader = [[VRTImageAsyncLoader alloc] initWithDelegate:self];
  _defaultSourceLoader.tag = kDefaultSource;
  _gazeSourceLoader.tag = kGazeSource;
  _tapSourceLoader.tag = kTapSource;
  return self;
}

- (void)viewWillAppear{
  self.defaultScale = self.node->getScale();
}

- (void)onTapDown {
  if (_tapMaterial != nil){
    [self node]->getGeometry()->getMaterials().clear();
    [self node]->getGeometry()->getMaterials().push_back(_tapMaterial);
  }
  self.node->setScale(self.defaultScale.scale(BUTTON_PRE_BOUNCE_SCALE));
}

- (void)onTapUp {
  /*
   * TODO: Tie the setting of gazing material to the end of the onTapAnimateBounce callback
   * once we have implemented animation callback functions.
   */
  if (_gazeMaterial != nil || _defaultMaterial != nil) {
    dispatch_time_t onTapUpTime = dispatch_time(DISPATCH_TIME_NOW, ON_TAP_UP_CALLBACK_SEC);
    dispatch_after(onTapUpTime, dispatch_get_main_queue(), ^(void) {
      [self node]->getGeometry()->getMaterials().clear();
      if(_gazeMaterial) {
        [self node]->getGeometry()->getMaterials().push_back(_gazeMaterial);
      }else if(_defaultMaterial) {
         //set back to our default material if no gaze material is specified.
         [self node]->getGeometry()->getMaterials().push_back(_defaultMaterial);
      }
    });
  }

  [self onTapUpAnimateBounce];
}

- (void)onTapUpAnimateBounce {
  VROTransaction::begin();
  VROTransaction::setAnimationDuration(BUTTON_BOUNCE_DURATION_SEC);
  VROTransaction::setTimingFunction(VROTimingFunctionType::Bounce);
  self.node->setScale(self.defaultScale);
  VROTransaction::commit();
}

- (void)onGazeStart {
  if (_gazeMaterial){
    [self node]->getGeometry()->getMaterials().clear();
    [self node]->getGeometry()->getMaterials().push_back(_gazeMaterial);
  }
}

- (void)onGazeEnd {
  if(_defaultMaterial) {
    [self node]->getGeometry()->getMaterials().clear();
    [self node]->getGeometry()->getMaterials().push_back(_defaultMaterial);
  }
}

/**
 * Re-creates gaze, tap and default textures with the provided material by copying the
 * material properties and changing just the diffuse texture.
 */
- (void)setMaterials:(NSArray<NSString *> *)materialArray {
  [super setMaterials:materialArray];
  if (_gazeTexture){
    _gazeMaterial = [self createMaterial:_gazeTexture];
  }
  if(_tapTexture) {
    _tapMaterial = [self createMaterial:_tapTexture];
  }
  if(_defaultTexture) {
    _defaultMaterial = [self createMaterial:_defaultTexture];
  }
}

/**
 * Loads the default source using async loader and updates the geometry with the default image.
 * NOTE: It is possible for the user to gaze at the button before the default image is loaded
 * (For example, large default images that takes a long time to load). However, this situation
 * is easily recoverable and may not be common one.
 */
- (void)setSource:(RCTImageSource *)imageSource {
  _source = imageSource;
  if(_source) {
    [_defaultSourceLoader loadImage:_source];
  }
}

// Load gaze source using async loader
- (void)setGazeSource:(RCTImageSource *)source {
  _gazeSource = source;
  if(_gazeSource) {
    [_gazeSourceLoader loadImage:_gazeSource];
  }
}

// Load gaze source using async loader
- (void)setTapSource:(RCTImageSource *)source {
  _tapSource = source;
  if(_tapSource) {
    [_tapSourceLoader loadImage:_tapSource];
  }
}

#pragma mark - VRTAsyncLoaderEventDelegate

- (void)imageLoaderDidStart:(VRTImageAsyncLoader *)loader {
  //no-op
}

- (void)imageLoaderDidEnd:(VRTImageAsyncLoader *)loader success:(BOOL)success image:(UIImage *)image {
  if(success && image!=nil) {
    if([loader.tag isEqualToString:kGazeSource]) {
      _gazeTexture = std::make_shared<VROTexture>(std::make_shared<VROImageiOS>(image));
      _gazeMaterial = [self createMaterial:_gazeTexture];
    }
    else if([loader.tag isEqualToString:kTapSource]) {
      _tapTexture = std::make_shared<VROTexture>(std::make_shared<VROImageiOS>(image));
      _tapMaterial = [self createMaterial:_tapTexture];
    }
    else if([loader.tag isEqualToString:kDefaultSource]) {
      _defaultTexture = std::make_shared<VROTexture>(std::make_shared<VROImageiOS>(image));
      _defaultMaterial = [self createMaterial:_defaultTexture];
      [self node]->getGeometry()->getMaterials().clear();
      [self node]->getGeometry()->getMaterials().push_back(_defaultMaterial);
    }
  } else {
      NSLog(@"ERROR: Failed to load button image for %@", loader.tag);
  }
}

- (std::shared_ptr<VROMaterial>)createMaterial:(std::shared_ptr<VROTexture>)texture {
  std::shared_ptr<VROMaterial> newMaterial;
  if(self.materials) {
     VROMaterialManager *materialManager = [self.bridge moduleForClass:[VROMaterialManager class]];
    // = [self.bridge materialManager];
    std::shared_ptr<VROMaterial> setMaterial = [materialManager getMaterialByName:self.materials[0]];
    // Create a copy of the material property within which we can set the diffuse content.
    newMaterial = std::make_shared<VROMaterial>(setMaterial);
  } else {
    newMaterial = std::make_shared<VROMaterial>();
  }
  newMaterial->getDiffuse().setTexture(texture);
  return newMaterial;
}

-(void)setCanHover:(BOOL)canHover {
    [super setCanHover: _gazeMaterial != NULL || canHover];
}

-(void)onClick:(int)source clickState:(VROEventDelegate::ClickState)clickState{
    [super onClick:source clickState:clickState];
    
    /*
     * Because Cardboard doesn't have onTap up events, we simulate the tapping event upon
     * a button by calling onTapDown then an onTapUp event. We can tie the down and up
     * events once they are available.
     */
    [self onTapDown];
    [self onTapUp];
}

-(void)onHover:(int)source isHovering:(bool)isHovering {
    [super onHover:source isHovering:isHovering];
    isHovering? [self onGazeStart] : [self onGazeEnd];
}

@end
