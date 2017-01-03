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
#import "VRTButton.h"
#import "VRTHUD.h"
#import "RCTSceneController.h"
#import "RCTConvert.h"
#import "VRTNode.h"
#import "VRTFlexView.h"
#import "VRTLabel.h"
#import "VROUtils.h"
#import "VRTAnimatedComponent.h"
#import "VROMaterialManager.h"

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
    NSArray *subsubViews = [vroView reactSubviews];
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
      VROMaterialManager *materialManager = [self.bridge moduleForClass:[VROMaterialManager class]];
      std::vector<std::shared_ptr<VROMaterial>> tempMaterials;

      for(NSString *materialName in self.materials) {
        std::shared_ptr<VROMaterial> material = [materialManager getMaterialByName:materialName];
        if(material != NULL) {
          tempMaterials.push_back(material);
        } else {
          RCTLogError(@"Unknown Material Name: \"%@\"", materialName);
          return;
        }
      }

      if (tempMaterials.size() > 0) {
        geometry->getMaterials() = tempMaterials;
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
  
  VRTNode *superview = nil;
  if ([self superview] && [[self superview] isKindOfClass:[VRTNode class]]) {
    superview = (VRTNode *)[self superview];
  } else if([self superview] && [[self superview] isKindOfClass:[VRTAnimatedComponent class]]) {
    if([[[self superview] superview] isKindOfClass:[VRTNode class]]) {
      superview = (VRTNode *)[[self superview] superview];
    }
  }
  
  //the 2d center of the superview, ie if the parent has a width and height of 5000 points, this is: 2500,2500.
  CGPoint centerPointParent2d = [superview centerPoint2D];
  
  //the 2d bounds, width and height of parent.
  CGRect boundsParent2d = [superview bounds2D];
  
  if(superview != nil) {
    VROVector3f superviewPos = superview.node->getPosition();
  }
  //flip y because our y increases as it goes 'up', instead of increasing downward with mobile.
  CGFloat transformedY = boundsParent2d.size.height - position.y;
  
  //transform by subtracting from center of superview.
  CGFloat transformedX = position.x - centerPointParent2d.x;
  transformedY = transformedY - centerPointParent2d.y;
  
  //Now make into 3d bounds and 3d position
  CGFloat width3d = bounds.size.width / k2DPointsPerSpatialUnit;
  CGFloat height3d = bounds.size.height / k2DPointsPerSpatialUnit;
  
  //multiply by height and width of parent to get correct position
  transformedX /= k2DPointsPerSpatialUnit;
  transformedY /= k2DPointsPerSpatialUnit;
  
  // Always place the children of views .01 in front of the parent. This helps with z-fighting and ensures that the child is always in front of the parent for hit detection
  float zIncrementToAvoidZFighting = 0.01;
  
  //VA: Uncomment when flexbox ready[self node]->setPosition({(float)transformedX, (float)transformedY, zIncrementToAvoidZFighting});
  CGPoint scale = [self fudgeFlexboxScaleX:width3d  Y:height3d];
  
  // Since VRTFlexView containers are actual size using width and height, set child components to appopriate width/height. If components don't have width/height attrib, use scale for now.
  if([self isKindOfClass:[VRTImage class]]) {
    VRTImage *image = (VRTImage *)self;
    //VA: Uncomment when flexbox ready...[image setWidth:bounds.size.width/ k2DPointsPerSpatialUnit];
    //VA: Uncomment when flexbox ready...[image setHeight:bounds.size.height/ k2DPointsPerSpatialUnit];
  } else if([self isKindOfClass:[VRTFlexView class]]) {
    VRTFlexView *flexview = (VRTFlexView *)self;
    [flexview setWidth:bounds.size.width/ k2DPointsPerSpatialUnit];
    [flexview setHeight:bounds.size.height/ k2DPointsPerSpatialUnit];
  }
  else {
    //VA: Uncomment when flexbox ready[self node]->setScale({(float)scale.x, (float)scale.y, 1.0});
  }

  // Avoid crashes due to nan coords
  if (isnan(position.x) || isnan(position.y) ||
      isnan(bounds.origin.x) || isnan(bounds.origin.y) ||
      isnan(bounds.size.width) || isnan(bounds.size.height)) {
    RCTLogError(@"Invalid layout for (%@)%@. position: %@. bounds: %@",
                self.reactTag, self, NSStringFromCGPoint(position), NSStringFromCGRect(bounds));
    return;
  }
  
  self.centerPoint2D = CGPointMake(bounds.size.width/2, bounds.size.height/2);
  self.bounds2D = bounds;
}

-(CGPoint)fudgeFlexboxScaleX:(float)width3d  Y:(float)height3d {
  return CGPointMake(width3d, height3d);
}

- (BOOL)isRootFlexboxView {
  return NO;
}

// Default implementation returns no.
- (BOOL)didViewHit:(VROHitTestResult)hitResult {
  if (self.node.get() == hitResult.getNode().get()) {
    return YES;
  }
  return NO;
}

// Execute the given hit event
- (BOOL)invokeHitEvent {
  // if an onTap function was provided in React, then return YES to signal that we've handled the onTap event.
  if (self.onTap && self.delegate) {
    [self.delegate onTap:self];
    return YES;
  }
  return NO;
}

// Check if this node has an _onGaze function.
- (BOOL)hoverable {
  return self.onGaze ? YES : NO;
}

@end
