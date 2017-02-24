//
//  VRTScene.m
//  React
//
//  Created by Vik Advani on 12/11/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#import <ViroKit/ViroKit.h>
#import <React/RCTConvert.h>
#import "VRTScene.h"
#import "VRTCamera.h"
#import "VRTOrbitCamera.h"
#import "VRTMaterialManager.h"

#import "VRTNode.h"
#import "VRTLog.h"
#import "VRTTreeNode.h"
#import <map>

static NSString *const kSizeKey = @"size";
static NSString *const kWallMaterialKey = @"wallMaterial";
static NSString *const kCeilingMaterialKey = @"ceilingMaterial";
static NSString *const kFloorMaterialKey = @"floorMaterial";
static NSString *const kDefaultMaterial = @"transparent";
static NSArray<NSNumber *> *const kDefaultSize = @[@(0), @(0), @(0)];

@implementation VRTScene {
  id <VROView> _vroView;
  VRTCamera *_camera;
  std::map<std::shared_ptr<VRONode>, float> _storedRootNodeOpacities;
  std::shared_ptr<VROSceneControllerDelegateiOS> _delegate;
  std::shared_ptr<VROSceneController> _sceneController;
  NSArray<NSNumber *> *_size;
  NSString *_wallMaterial;
  NSString *_ceilingMaterial;
  NSString *_floorMaterial;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge  {
  self = [super initWithBridge:bridge];
  // Create VROSceneController.
  _sceneController = std::make_shared<VROSceneController>();

  // Create and attach delegate
  _delegate = std::make_shared<VROSceneControllerDelegateiOS>(self);
  _sceneController->setDelegate(_delegate);

  //Set root node for this scene
  _sceneController->getScene()->addNode(self.node);
  if (self) {
    _recticleEnabled = true;
  }
    
  _size = kDefaultSize;
  _wallMaterial = kDefaultMaterial;
  _ceilingMaterial = kDefaultMaterial;
  _floorMaterial = kDefaultMaterial;
  return self;
}

- (void)dealloc {
  
}

- (void)setView:(id <VROView>)view {
  _vroView = view;
  [self setCameraIfAvailable];
}

-(void)setDriver:(VRODriver *)driver {
    [super setDriver:driver];
    self.driver->setSoundRoom([[_size objectAtIndex:0] floatValue], [[_size objectAtIndex:1] floatValue], [[_size objectAtIndex:2] floatValue], [_wallMaterial UTF8String], [_ceilingMaterial UTF8String], [_floorMaterial UTF8String]);
}

- (std::shared_ptr<VROSceneController>)sceneController {
    return _sceneController;
}

- (std::shared_ptr<VROScene>)scene {
    return _sceneController->getScene();
}

- (void)setSoundRoom:(NSDictionary *)soundRoom {
    if (soundRoom) {
        _size = [soundRoom objectForKey:kSizeKey] ? [soundRoom objectForKey:kSizeKey] : kDefaultSize;
        _wallMaterial = [soundRoom objectForKey:kWallMaterialKey] ? [soundRoom objectForKey:kWallMaterialKey] : kDefaultMaterial;
        _ceilingMaterial = [soundRoom objectForKey:kCeilingMaterialKey] ? [soundRoom objectForKey:kCeilingMaterialKey] : kCeilingMaterialKey;
        _floorMaterial = [soundRoom objectForKey:kFloorMaterialKey] ? [soundRoom objectForKey:kFloorMaterialKey] : kFloorMaterialKey;
    } else {
        _size = kDefaultSize;
        _wallMaterial = kDefaultMaterial;
        _ceilingMaterial = kDefaultMaterial;
        _floorMaterial = kDefaultMaterial;
    }

    if (self.driver) {
        self.driver->setSoundRoom([[_size objectAtIndex:0] floatValue], [[_size objectAtIndex:1] floatValue], [[_size objectAtIndex:2] floatValue], [_wallMaterial UTF8String], [_ceilingMaterial UTF8String], [_floorMaterial UTF8String]);
    }
}

-(void)setOnPlatformUpdateViro:(RCTDirectEventBlock)onPlatformUpdateViro {
    _onPlatformUpdateViro = onPlatformUpdateViro;
    // since we're on iOS, the platform information is all known, so simply call the function as soon as it is set.
    if (_onPlatformUpdateViro) {
        _onPlatformUpdateViro(@{@"platformInfoViro": @{@"vrPlatform" : @"gvr",
                                                       @"headset" : @"cardboard",
                                                       @"controller" : @"cardboard"}});
    }
}

#pragma mark - Camera

- (void)setCameraIfAvailable {
    if (!_camera || !_vroView) {
        return;
    }
    
    [_vroView setPointOfView:_camera.node];
    NSLog(@"Set camera with rotation type %d", _camera.node->getCamera()->getRotationType());
}

- (void)setCamera:(VRTCamera *)camera {
  _camera = camera;
  [self setCameraIfAvailable];
}

- (void)removeCamera:(VRTCamera *)camera {
  if (_camera != camera) {
      return;
  }
    
  if (_vroView) {
    [_vroView setPointOfView:nullptr];
  }
  _camera = nil;
}

#pragma mark - Scene-specific subviews

- (void)insertReactSubview:(UIView *)view atIndex:(NSInteger)atIndex {
  [super insertReactSubview:view atIndex:atIndex];
}

- (void)removeReactSubview:(UIView *)subview {
  [super removeReactSubview:subview];
}

#pragma mark - Property setter overrides

/**
 * Override the cameraPosition getter to return the renderer's actual camera position vs
 * the property value which is the position the developer set.
 */
- (NSArray<NSNumber *> *)cameraPosition {
  if (self.context) {
    VROVector3f actualCameraPosition = self.context->getCamera().getPosition();
    return @[@(actualCameraPosition.x), @(actualCameraPosition.y), @(actualCameraPosition.z)];
  } else {
    return @[@0, @0, @0];
  }
}

#pragma mark - VROSceneDelegateiOS methods.

- (void)sceneWillAppear:(VRORenderContext *)context driver:(VRODriver *)driver {
  self.context = context;
  self.driver = driver;
    
  for(VRTView *view in _childViews) {
    view.context = context;
    view.driver = driver;
    view.scene = [self scene];
    
    [view viewWillAppear];
  }
}

- (void)sceneDidAppear:(VRORenderContext *)context driver:(VRODriver *)driver {
  
}

- (void)sceneWillDisappear:(VRORenderContext *)context driver:(VRODriver *)driver {
  self.driver = driver;
  for(VRTView *view in _childViews) {
    [view viewWillDisappear];
  }
}

- (void)sceneDidDisappear:(VRORenderContext *)context driver:(VRODriver *)driver {

}

- (void)sceneWillRender:(const VRORenderContext *)context {
  self.context = (VRORenderContext *)context;
}

/**
 * Log out debug information specific to this scene, such as Camera forward vector
 * in cartesian and polar formats.
 **/
- (void)logSceneDebugInfo:(const VRORenderContext *)context{
  VROVector3f forwardVector = context->getCamera().getForward().normalize();
  float r = sqrtf((pow(forwardVector.x, 2)) + (pow(forwardVector.y, 2))+ (pow(forwardVector.z, 2)));
  float theta = toDegrees(atan(forwardVector.z / forwardVector.x));
  float phi = toDegrees(acos(forwardVector.y / r));

  /* Required offsets because of the Right Hand Rule used for our coordinate system for Theta.
   * Currently, in the XZ plane, the observed vector and theta angles in in the following format:
   *          (-z)
   *        90 | -90
   * 0         |        0
   * (-x) - - - - - - (+x)
   * 0         |        0
   *        -90| 90
   *          (+z)
   * As such, we'll need to apply the following offsets:
   */
  if (forwardVector.x < 0){
    theta = theta + 180;
  } else if (forwardVector.z < 0){
    theta = theta + 360;
  }
  
  NSString *log = [NSString stringWithFormat:
          @"\nCamera Polar Coordinates: Theta %f, Phi %f. \nCamera Forward Vector %f, %f, %f",
          theta, phi, forwardVector.x, forwardVector.y, forwardVector.z];
  [VRTLog debug:log];
}

@end
