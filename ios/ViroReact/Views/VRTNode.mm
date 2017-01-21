//
//  VRTNode.m
//  React
//
//  Created by Raj Advani on 8/24/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTNode.h"
#import "VRTScene.h"
#import "VRTLight.h"
#import "VRTImage.h"
#import "VRTHUD.h"
#import "RCTConvert.h"
#import "VRTNode.h"
#import "VRTFlexView.h"
#import "VRTText.h"
#import "VRTUtils.h"
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

-(void)setOnTapViro:(RCTDirectEventBlock)block {
    _onTapViro = block;
}

-(void)setOnGazeViro:(RCTDirectEventBlock)block {
    _onGazeViro = block;
}

-(void)setCanGaze:(BOOL)canGaze {
    _canGaze = canGaze;
    self.eventDelegate->setEnabledEvent(VROEventDelegate::EventType::ON_GAZE, canGaze);
}

-(void)setCanTap:(BOOL)canTap {
    _canTap = canTap;
    self.eventDelegate->setEnabledEvent(VROEventDelegate::EventType::ON_TAP, canTap);
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
    if ([behavior caseInsensitiveCompare:@"billboardZ"] == NSOrderedSame) {
      [self node]->addConstraint(std::make_shared<VROBillboardConstraint>(VROBillboardAxis::Z));
    }
  }
}

- (void)setMaterials:(NSArray<NSString *> *)materials {
  _materials = materials;
  [self applyMaterials];
}

- (void)applyMaterials {
  std::shared_ptr<VRONode> node = self.node;
  if (node) {
    std::shared_ptr<VROGeometry> geometry = node->getGeometry();
    
    if (geometry) {
      VRTMaterialManager *materialManager = [self.bridge moduleForClass:[VRTMaterialManager class]];
      
        for (int i = 0; i < self.materials.count; i++) {
            NSString *materialName = [self.materials objectAtIndex:i];
            std::shared_ptr<VROMaterial> material = [materialManager getMaterialByName:materialName];
            if (material == NULL) {
                RCTLogError(@"Unknown Material Name: \"%@\"", materialName);
                return;
            }

            // Copy the material into the geometry if the geometry already has a
            // default material in that slot. Otherwise copy the material and add it
            // to the geometry.
            if (i < geometry->getMaterials().size()) {
                geometry->getMaterials()[i]->copyFrom(material);
            }
            else {
                geometry->getMaterials().push_back(std::make_shared<VROMaterial>(material));
            }
        }
    }
  }
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

-(void)onTapped {
    if (self.onTapViro != nil) {
        self.onTapViro(@{@"tapped": @(true),});
    }
}
-(void)onGaze:(BOOL)isGazing {
    if (self.onTapViro != nil) {
        self.onGazeViro(@{@"isGazing": @(isGazing),});
    }
}

@end
