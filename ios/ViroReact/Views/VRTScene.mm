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
#import "VROMaterialManager.h"
#import "RCTSceneController.h"
#import "RCTConvert.h"
#import "VRTNode.h"
#import "VRTLog.h"
#import "VRTTreeNode.h"
#import <map>


@implementation VRTScene {
  
  id <VROView> _vroView;
  VRTCamera *_camera;
  std::map<std::shared_ptr<VRONode>, float> _storedRootNodeOpacities;
  
}

- (instancetype)initWithBridge:(RCTBridge *)bridge  {
  self = [super initWithBridge:bridge];
  if (self) {
    _recticleEnabled = true;
  }
  return self;
}

- (void)dealloc {
  
}

- (void)setView:(id <VROView>)view {
  _sceneController = [[RCTSceneController alloc] initWithView:view sceneDelegate:self];
  [_sceneController setHoverEnabled:YES boundsOnly:YES];
  _sceneController.scene->addNode(self.node);
  _vroView = view;
  [self setCameraIfAvailable];
  _vroView.reticle->setEnabled(_recticleEnabled);
}

-(void)setReticleEnabled:(BOOL)enabled{
  _recticleEnabled = enabled;
  if (_vroView){
    _vroView.reticle->setEnabled(enabled);
  }
}

- (std::shared_ptr<VROScene>)scene {
  return _sceneController.scene;
}

#pragma mark - Scene plumbing to VROView

- (void)setCameraIfAvailable {
  if (_camera && _vroView) {
    NSArray <NSNumber *> *positionArray = _camera.position;
    VROVector3f position = VROVector3f([positionArray[0] floatValue],
                                       [positionArray[1] floatValue],
                                       [positionArray[2] floatValue]);
    [_vroView setPosition:position];
    
    VROCameraRotationType rotationType = [_camera rotationType];
    [_vroView setCameraRotationType:rotationType];
    
    if (rotationType == VROCameraRotationType::Orbit) {
      VRTOrbitCamera *orbitCamera = (VRTOrbitCamera *) _camera;
      
      NSArray <NSNumber *> *focalPointArray = orbitCamera.focalPoint;
      VROVector3f focalPoint = VROVector3f([focalPointArray[0] floatValue],
                                           [focalPointArray[1] floatValue],
                                           [focalPointArray[2] floatValue]);
      [_vroView setOrbitFocalPoint:focalPoint];
    }
  }
}

#pragma mark - Scene-specific subviews

- (void)insertReactSubview:(UIView *)view atIndex:(NSInteger)atIndex {
  VRTView *component = (VRTView *)view;

  if ([component isKindOfClass:[VRTCamera class]]) {
    _camera = (VRTCamera *)component;
    [self setCameraIfAvailable];
  }
  
  [super insertReactSubview:view atIndex:atIndex];
}

- (void)removeReactSubview:(UIView *)subview {
  if ([subview isKindOfClass:[VRTCamera class]]) {
    _camera = nil;
  }
  
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

#pragma mark - VRTSceneDelegate methods.

- (void)sceneWillAppear:(VRORenderContext *)context driver:(VRODriver *)driver {
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
 * This function takes a VRONode and checks whether it is hoverable by finding its
 * corresponding VRTNode and asking the VRTNode if it is hoverable.
 */
- (BOOL)isHoverable:(std::shared_ptr<VRONode>)node {
  VRTView *candidate = [self findNode:node fromVrtView:self];
  return candidate ? [candidate hoverable] : NO;
}

- (void)hoverOnNode:(std::shared_ptr<VRONode>)node {
  [self hoverNode:node onNode:YES];
}

- (void)hoverOffNode:(std::shared_ptr<VRONode>)node {
  [self hoverNode:node onNode:NO];
}

/**
 * This function is called when the given node is either hovered on or off.
 *
 * node - the node whose hover state changed.
 * onNode - whether or not the hovering is on or off the given node.
 */
- (void)hoverNode:(std::shared_ptr<VRONode>)node onNode:(BOOL)onNode {
  // If node is null, nothing is being hovered upon, and the user
  // is currently focusing on the background of the current scene.
  if (!node){
      [self.delegate onGaze:self state:onNode];
      return;
  }

  VRTView *candidate = [self findNode:node fromVrtView:self];
  if (candidate) {
      [candidate.delegate onGaze:candidate state:onNode];
  }
}

/**
 * This function takes a VRONode and attempts to find a VRTView that corresponds
 * to it starting from the given VRTView.
 */
- (VRTView *)findNode:(std::shared_ptr<VRONode>)node fromVrtView:(VRTView *)view {
  // only VRTNodes have a corresponding VRONode
  if ([view isKindOfClass:[VRTNode class]]) {
    VRTNode *nodeView = (VRTNode *)view;
    if (nodeView.node.get() == node.get()) {
      return nodeView;
    }
  }
  for (VRTView *subView in [view reactSubviews]) {
    VRTView *candidate = [self findNode:node fromVrtView:subView];
    if (candidate) {
      return candidate;
    }
  }
  // make sure we check all child views before we give up on this subtree
  return nil;
}

/**
 * This function is invoked when the reticle is tapped. It first asks the renderer for all
 * the views which were intersected by the given ray representing the camera direction. It
 * then creates a z-depth tree based on the hit results and iterates over the tree in a
 * depth first order while calling 'invokeHitEvent' on all the views until one responds.
 */
- (void)reticleTapped:(VROVector3f)ray context:(const VRORenderContext *)context {
  // fetch all the views in increasing distance from the camera that intersects with the given ray.
  std::vector<VROHitTestResult> results = _sceneController.scene->hitTest(ray, *context, false);

  // Log some debug info if debug mode is enabled.
  if ([VRTLog debugEnabled]){
    [self logSceneDebugInfo:context];
  }

  // For each hit, get the path to it from the scene. We should ALWAYS be able to
  // find the view.
  NSMutableArray<NSArray<VRTView *> *> *pathsToResults = [[NSMutableArray alloc] init];
  for (VROHitTestResult result: results) {
    NSArray<VRTView *> *pathToResult = [self viewHierarchyOfResult:result];
    if (!pathToResult) {
      RCTLogError(@"Hit result doesn't exist in view hierarchy!");
    }
    [pathsToResults addObject:pathToResult];
  }

  // Create a tree out of the path to each hit result.
  VRTTreeNode *zDepthTree = [[VRTTreeNode alloc] initWithValue:self];

  for (NSArray *path: pathsToResults) {
    VRTTreeNode *currentNode = zDepthTree;
    for (int i = 1; i < path.count; i++) {
      VRTTreeNode *childNode = [currentNode childWithValue:path[i]];
      if (!childNode) {
        childNode = [[VRTTreeNode alloc] initWithValue:path[i]];
        [currentNode addChild:childNode];
      }
      currentNode = childNode;
    }
  }

  // Perform a depth-first ordered iteration over the tree.
  [self invokeHitEventOnTree:zDepthTree];
}

/**
 * Private helper function to call invokeHitEvent on the given tree depth-first and stopping
 * as soon as any view/node has handled the event.
 */
- (BOOL)invokeHitEventOnTree:(VRTTreeNode *)tree {
  for (VRTTreeNode *childNode: [tree getChildren]) {
    if ([self invokeHitEventOnTree:childNode]) {
      return YES;
    }
  }
  // base case after the recursion makes this depth-first.
  VRTView *rootView = (VRTView *)tree.value;
  return [rootView invokeHitEvent];
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
