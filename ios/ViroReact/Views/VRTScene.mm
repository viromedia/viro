//
//  VRTScene.m
//  React
//
//  Created by Vik Advani on 12/11/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#import <ViroKit/ViroKit.h>
#import "VRTScene.h"
#import "VRTCamera.h"
#import "VRTOrbitCamera.h"
#import "VRTMaterialManager.h"
#import "RCTConvert.h"
#import "VRTNode.h"
#import "VRTLog.h"
#import "VRTTreeNode.h"
#import <map>

@implementation VRTScene {
  id <VROView> _vroView;
  VRTCamera *_camera;
  std::map<std::shared_ptr<VRONode>, float> _storedRootNodeOpacities;
  std::shared_ptr<VROSceneControllerDelegateiOS> _delegate;
  std::shared_ptr<VROSceneController> _sceneController;
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
  return self;
}

- (void)dealloc {
  
}

- (void)setView:(id <VROView>)view {
  _vroView = view;
  [self setCameraIfAvailable];
  _vroView.reticle->setEnabled(_recticleEnabled);
}

-(void)setReticleEnabled:(BOOL)enabled {
  _recticleEnabled = enabled;
  if (_vroView){
    _vroView.reticle->setEnabled(enabled);
  }
}

- (std::shared_ptr<VROSceneController>)sceneController {
    return _sceneController;
}

- (std::shared_ptr<VROScene>)scene {
    return _sceneController->getScene();
}

#pragma mark - Camera

- (void)setCameraIfAvailable {
    if (!_camera || !_vroView) {
        return;
    }
    
    VRTView *pointOfView = [_camera superview];
    if (![pointOfView isKindOfClass:[VRTNode class]]) {
        RCTLogError(@"Camera superview is not a VRTNode!");
        return;
    }
    
    VRTNode *node = (VRTNode *)pointOfView;
    [_vroView setPointOfView:node.node];
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

- (void)startIncomingTransition:(VRORenderContext *)context duration:(float)duration {
  
  if (self.scene->getBackground()) {
    self.scene->getBackground()->getMaterials().front()->setTransparency(0.0);
  }
  
  std::map<std::shared_ptr<VRONode>, float> finalOpacity;
  if(_storedRootNodeOpacities.size() > 0 ) {
    //set the final opacity to what we stored when this scene was previously displayed. This should be safe since
    //the scene itself is the root node.
    for(std::shared_ptr<VRONode> root :  self.scene->getRootNodes()) {
       finalOpacity[root] = _storedRootNodeOpacities[root];
      root->setOpacity(0.0f);
    }
  } else {
    for (std::shared_ptr<VRONode> root : self.scene->getRootNodes()) {
      float opacity = root->getOpacity();
      finalOpacity[root] = opacity;
      root->setOpacity(0.0f);
    }
  }
  
  VROTransaction::begin();
  VROTransaction::setAnimationDuration(duration);
  VROTransaction::setTimingFunction(VROTimingFunctionType::EaseIn);
  
  for (std::shared_ptr<VRONode> root : self.scene->getRootNodes()) {
    float opacity = finalOpacity[root];
    root->setOpacity(opacity);
  }
  if (self.scene->getBackground()) {
    self.scene->getBackground()->getMaterials().front()->setTransparency(1.0);
  }
  
  VROTransaction::commit();
}

- (void)startOutgoingTransition:(VRORenderContext *)context duration:(float)duration {
  // Default animation
  
  for (std::shared_ptr<VRONode> root : self.scene->getRootNodes()) {
    //store the opacity of the roots nodes, so when we come back to this scene we know what they should be.
    _storedRootNodeOpacities[root] = root->getOpacity();
  }
  VROTransaction::begin();
  VROTransaction::setAnimationDuration(duration);
  VROTransaction::setTimingFunction(VROTimingFunctionType::EaseIn);
  
  for (std::shared_ptr<VRONode> root : self.scene->getRootNodes()) {
    root->setOpacity(0.0f);
  }
  if (self.scene->getBackground()) {
    self.scene->getBackground()->getMaterials().front()->setTransparency(0.0);
  }
  
  VROTransaction::commit();
}

- (void)endIncomingTransition:(VRORenderContext *)context {
  
}

- (void)endOutgoingTransition:(VRORenderContext *)context {
  
}

- (void)animateIncomingTransition:(VRORenderContext *)context percentComplete:(float)t {
  
}

- (void)animateOutgoingTransition:(VRORenderContext *)context percentComplete:(float)t {
  
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
