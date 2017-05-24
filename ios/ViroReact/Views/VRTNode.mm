//
//  VRTNode.m
//  React
//
//  Created by Raj Advani on 8/24/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <React/RCTConvert.h>
#import "VRTNode.h"
#import "VRTScene.h"
#import "VRTLight.h"
#import "VRTImage.h"
#import "VRTHUD.h"
#import "VRTNode.h"
#import "VRTFlexView.h"
#import "VRTText.h"
#import "VRTUtils.h"
#import "VRTCamera.h"
#import "VRTAnimatedComponent.h"
#import "VRTMaterialManager.h"

const int k2DPointsPerSpatialUnit = 1000;

@implementation VRTNode

- (instancetype)initWithBridge:(RCTBridge *)bridge  {
  self = [super initWithBridge:bridge];
  if(self) {
    _node = std::make_shared<VRONode>();
    _node->setPosition({0, 0, 0});
    _node->setScale({1, 1, 1});
    _visible = YES; // default to visible.
    _opacity = 1.0; //default opacity to 1.0
    _highAccuracyGaze = NO;
      
    // Create and attach event delegate
    _eventDelegate = std::make_shared<VROEventDelegateiOS>(self);
    _node->setEventDelegate(_eventDelegate);
  }
  
  return self;
}

- (void)insertReactSubview:(UIView *)view atIndex:(NSInteger)atIndex {
  VRTView *vroView = (VRTView *)view;
  
  if ([vroView isKindOfClass:[VRTLight class]]) {
    VRTLight *light = (VRTLight *)vroView;
    self.node->addLight([light light]);
  }
  
  else if ([vroView isKindOfClass:[VRTNode class]]) {
    VRTNode *nodeView = (VRTNode *)vroView;
    self.node->addChildNode(nodeView.node);
  }
  
  else if ([vroView isKindOfClass:[VRTAnimatedComponent class]]) {
    /*
     Add all children (the targets of the animation) to the node.
     */
    NSArray *subsubViews = [vroView reactSubviews];
    BOOL childFound = false;
    
    for(VRTView *subsubview in subsubViews){
      if (![subsubview isKindOfClass:[VRTNode class]]) {
        continue;
      }
      
      VRTNode *subsubNodeView = (VRTNode *)subsubview;
      
      std::vector<std::shared_ptr<VRONode>> subnodeArray = self.node->getSubnodes();
      for(std::shared_ptr<VRONode> node: subnodeArray){
        if(node.get() == subsubNodeView.node.get()){
          childFound = true;
          break;
        }
      }
      
      if(!childFound){
        self.node->addChildNode(subsubNodeView.node);
      }
    }
  }
  
  [super insertReactSubview:view atIndex:atIndex];
}

- (void)removeReactSubview:(UIView *)subview {
  VRTView *vroView = (VRTView *)subview;
  
  if ([vroView isKindOfClass:[VRTLight class]]) {
    VRTLight *light = (VRTLight *)vroView;
    self.node->removeLight([light light]);
  }
  
  else if ([vroView isKindOfClass:[VRTNode class]]) {
    VRTNode *nodeView = (VRTNode *)vroView;
    nodeView.node->removeFromParentNode();
  }
  
  else if ([vroView isKindOfClass:[VRTAnimatedComponent class]]) {
    /*
     Remove the child (the target of the animation) from the node.
     */
    for(VRTView *subsubview in [vroView reactSubviews]) {
      if (![subsubview isKindOfClass:[VRTNode class]]) {
        continue;
      }
      
      VRTNode *subsubNodeView = (VRTNode *)subsubview;
      subsubNodeView.node->removeFromParentNode();
    }
  }
  
  [super removeReactSubview:subview];
}

-(void)onHoverViro:(RCTDirectEventBlock)block {
    _onHoverViro = block;
}

-(void)onClickViro:(RCTDirectEventBlock)block {
    _onClickViro = block;
}

-(void)setCanHover:(BOOL)canHover {
    _canHover = canHover;
    self.eventDelegate->setEnabledEvent(VROEventDelegate::EventAction::OnHover, canHover);
}

-(void)setCanClick:(BOOL)canClick {
    _canClick = canClick;
    self.eventDelegate->setEnabledEvent(VROEventDelegate::EventAction::OnClick, canClick);
}

-(void)setCanFuse:(BOOL)canFuse {
    _canFuse = canFuse;
    self.eventDelegate->setEnabledEvent(VROEventDelegate::EventAction::OnFuse, canFuse);
}

-(void)setTimeToFuse:(float)durationMillis {
    _timeToFuse = durationMillis;
    self.eventDelegate->setTimeToFuse(durationMillis);
}

- (void)setPosition:(NSArray<NSNumber *> *)position {
  _position = [position copy];
  float positionValues[3];
  populateFloatArrayFromNSArray(position, positionValues, 3);
  [self node]->setPosition({positionValues[0], positionValues[1], positionValues[2]});
}

- (void)setOpacity:(float)opacity {
  _opacity = opacity;
  [self node]->setOpacity(_opacity);
}

- (void)setRotation:(NSArray<NSNumber *> *)rotation {
  _rotation = [rotation copy];
  float rotationValues[3];
  populateFloatArrayFromNSArray(rotation, rotationValues, 3);
  [self node]->setRotation({toRadians(rotationValues[0]), toRadians(rotationValues[1]), toRadians(rotationValues[2])});
}

- (void)setScale:(NSArray<NSNumber *> *)scale {
  _scale = [scale copy];
  float scaleValues[3];
  populateFloatArrayFromNSArray(scale, scaleValues, 3);
  [self node]->setScale({scaleValues[0], scaleValues[1], scaleValues[2]});
}

- (void)setRotationPivot:(NSArray<NSNumber *> *)pivot {
  _rotationPivot = [pivot copy];
  float pivotValues[3];
  populateFloatArrayFromNSArray(pivot, pivotValues, 3);
    
  VROMatrix4f pivotMatrix;
  pivotMatrix.translate(pivotValues[0], pivotValues[1], pivotValues[2]);
  [self node]->setRotationPivot(pivotMatrix);
}

- (void)setScalePivot:(NSArray<NSNumber *> *)pivot {
  _scalePivot = [pivot copy];
  float pivotValues[3];
  populateFloatArrayFromNSArray(pivot, pivotValues, 3);
    
  VROMatrix4f pivotMatrix;
  pivotMatrix.translate(pivotValues[0], pivotValues[1], pivotValues[2]);
  [self node]->setScalePivot(pivotMatrix);
}

- (void)setTransformBehaviors:(NSArray<NSString *> *)behaviors {
  [self node]->removeAllConstraints();
  for (NSString *behavior in behaviors) {
    if ([behavior caseInsensitiveCompare:@"billboard"] == NSOrderedSame) {
      [self node]->addConstraint(std::make_shared<VROBillboardConstraint>(VROBillboardAxis::All));
    }
    if ([behavior caseInsensitiveCompare:@"billboardX"] == NSOrderedSame) {
      [self node]->addConstraint(std::make_shared<VROBillboardConstraint>(VROBillboardAxis::X));
    }
    if ([behavior caseInsensitiveCompare:@"billboardY"] == NSOrderedSame) {
      [self node]->addConstraint(std::make_shared<VROBillboardConstraint>(VROBillboardAxis::Y));
    }
  }
}

- (void)setMaterials:(NSArray<NSString *> *)materials {
  _materials = materials;
  [self applyMaterials];
}

// Apply materials to the underlying geometry if materials were explicitly set
// via the materials prop
- (void)applyMaterials {
  if (!self.node || !self.materials) {
    return;
  }
  
  std::shared_ptr<VROGeometry> geometry = self.node->getGeometry();
  if (!geometry) {
    return;
  }
  
  VRTMaterialManager *materialManager = [self.bridge moduleForClass:[VRTMaterialManager class]];

  std::vector<std::shared_ptr<VROMaterial>> tempMaterials;
  for (int i = 0; i < self.materials.count; i++) {
    NSString *materialName = [self.materials objectAtIndex:i];
    
    std::shared_ptr<VROMaterial> material = [materialManager getMaterialByName:materialName];
    if (material == NULL) {
      RCTLogError(@"Unknown Material Name: \"%@\"", materialName);
      return;
    }
    
    // Always copy materials from the material manager, as they may be
    // modified by animations, etc. and we don't want these changes to
    // propagate to the reference material held by the material manager
    tempMaterials.push_back(std::make_shared<VROMaterial>(material));
  }
  geometry->setMaterials(tempMaterials);
}

- (void)setVisible:(BOOL)visible {
  _visible = visible;
  [self node]->setHidden(!_visible);
}

- (void)setHighAccuracyGaze:(BOOL)enabled{
  _highAccuracyGaze = enabled;
  [self node]->setHighAccuracyGaze(enabled);
}

- (void)reactSetFrame:(CGRect)frame {
  // These frames are in terms of anchorPoint = topLeft, but internally the
  // views are anchorPoint = center for easier scale and rotation animations.
  // Convert the frame so it works with anchorPoint = center.
  CGPoint position = {CGRectGetMidX(frame), CGRectGetMidY(frame)};
  CGRect bounds = {CGPointZero, frame.size};
  
  self.position2DFlex = position;
  self.centerPoint2DFlex = CGPointMake(bounds.size.width/2, bounds.size.height/2);
  self.bounds2DFlex = bounds;
}

-(CGPoint)fudgeFlexboxScaleX:(float)width3d  Y:(float)height3d {
  return CGPointMake(width3d, height3d);
}

- (BOOL)isRootFlexboxView {
  return NO;
}

#pragma mark default implementations for VRTEventDelegateProtocol

-(void)onHover:(int)source isHovering:(bool)isHovering {
    if (self.onHoverViro != nil) {
        self.onHoverViro(@{@"source": @(source),
                         @"isHovering":@(isHovering)});
    }
}

-(void)onClick:(int)source clickState:(VROEventDelegate::ClickState)clickState{
    if (self.onClickViro != nil) {
        self.onClickViro(@{@"source": @(source),
                          @"clickState":@(clickState)});
    }
}

-(void)onFuse:(int)source{
    if (self.onFuseViro != nil) {
        self.onFuseViro(@{@"source": @(source)});
    }
}

@end
