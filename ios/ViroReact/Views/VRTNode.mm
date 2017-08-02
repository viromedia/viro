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
const double kTransformDelegateDistanceFilter = 0.01;
@implementation VRTNode

- (instancetype)initWithBridge:(RCTBridge *)bridge  {
  self = [super initWithBridge:bridge];
  if(self) {
    _node = [self createVroNode];
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

- (std::shared_ptr<VRONode>)createVroNode {
  return std::make_shared<VRONode>();
}
   
- (void)insertReactSubview:(UIView *)view atIndex:(NSInteger)atIndex {
  VRTView *child = (VRTView *)view;
    
  if ([child isKindOfClass:[VRTLight class]]) {
    VRTLight *light = (VRTLight *)child;
    self.node->addLight([light light]);
  } else if ([child isKindOfClass:[VRTNode class]]) {
    VRTNode *nodeView = (VRTNode *)child;
    self.node->addChildNode(nodeView.node);
  } else if ([child isKindOfClass:[VRTAnimatedComponent class]]) {
    /*
     Add all children (the targets of the animation) to the node.
     */
    NSArray *subsubViews = [child reactSubviews];
    BOOL childFound = false;

    for(VRTView *subsubview in subsubViews){
      if (![subsubview isKindOfClass:[VRTNode class]]) {
        continue;
      }

      VRTNode *subsubNodeView = (VRTNode *)subsubview;

      std::vector<std::shared_ptr<VRONode>> subnodeArray = self.node->getChildNodes();
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
    [nodeView clearPhysicsBody];
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

// Override parent shouldAppear function.
- (BOOL)shouldAppear {
    return self.parentHasAppeared && self.visible;
}

/*
 Careful to *NEVER* call parentDidAppear or parentDidDisappear within this
 function because only the parent view should ever call it.
 */
- (void)handleAppearanceChange {
    std::shared_ptr<VROPhysicsBody> body = self.node->getPhysicsBody();
    if (body) {
        body->setIsSimulated([self shouldAppear]);
    }
    [super handleAppearanceChange];    
}

- (void)setPosition:(NSArray<NSNumber *> *)position {
  _position = [position copy];
  float positionValues[3];
  populateFloatArrayFromNSArray(position, positionValues, 3);
  [self node]->setPosition({positionValues[0], positionValues[1], positionValues[2]});
}

-(void)setHasTransformDelegate:(BOOL)hasDelegate {
    if (hasDelegate){
        _transformDelegate = std::make_shared<VROTransformDelegateiOS>(self, kTransformDelegateDistanceFilter);
        [self node]->setTransformDelegate(_transformDelegate);
    } else {
        _transformDelegate = nullptr;
    }
}

- (void)onPositionUpdate:(VROVector3f)position{
    NSArray *array = [[NSArray alloc] initWithObjects:
                      [NSNumber numberWithFloat:position.x],
                      [NSNumber numberWithFloat:position.y],
                      [NSNumber numberWithFloat:position.z],
                      nil];
    
    if (self.onNativeTransformDelegateViro){
        self.onNativeTransformDelegateViro(@{@"position":array});
    }
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
  [self handleAppearanceChange];
  [self node]->setHidden(!_visible);
}

- (void)setViroTag:(NSString *)tag {
    std::string nodeTag;
    if (tag) {
        nodeTag = std::string([tag UTF8String]);
    }
    [self node]->setTag(nodeTag);
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

#pragma mark VRTEventDelegateProtocol Delegates
- (void)onHoverViro:(RCTDirectEventBlock)block {
    _onHoverViro = block;
}

- (void)onClickViro:(RCTDirectEventBlock)block {
    _onClickViro = block;
}

- (void)setCanCollide:(BOOL)canCollide {
    _canCollide = canCollide;

    if (canCollide && !_physicsDelegate) {
        _physicsDelegate = std::make_shared<VROPhysicsBodyDelegateiOS>(self);
    } else if (!canCollide) {
        _physicsDelegate = nil;
    }

    // Update the physic body's delegate if possible
    std::shared_ptr<VROPhysicsBody> body = [self node]->getPhysicsBody();
    if (!body) {
        return;
    }

    if (canCollide) {
        body->setPhysicsDelegate(_physicsDelegate);
    } else {
        body->setPhysicsDelegate(nullptr);
    }
}

- (void)setCanPinch:(BOOL)canPinch {
    _canPinch = canPinch;
    self.eventDelegate->setEnabledEvent(VROEventDelegate::EventAction::OnPinch, canPinch);
}

- (void)setCanHover:(BOOL)canHover {
    _canHover = canHover;
    self.eventDelegate->setEnabledEvent(VROEventDelegate::EventAction::OnHover, canHover);
}

- (void)setCanClick:(BOOL)canClick {
    _canClick = canClick;
    self.eventDelegate->setEnabledEvent(VROEventDelegate::EventAction::OnClick, canClick);
}

- (void)setCanFuse:(BOOL)canFuse {
    _canFuse = canFuse;
    self.eventDelegate->setEnabledEvent(VROEventDelegate::EventAction::OnFuse, canFuse);
}

- (void)setCanDrag:(BOOL)canDrag {
    _canDrag = canDrag;
    self.eventDelegate->setEnabledEvent(VROEventDelegate::EventAction::OnDrag, canDrag);
}

- (void)setTimeToFuse:(float)durationMillis {
    _timeToFuse = durationMillis;
    self.eventDelegate->setTimeToFuse(durationMillis);
}

-(void)onHover:(int)source
    isHovering:(bool)isHovering
 hoverLocation:(std::vector<float>)location {
    if (self.onHoverViro != nil) {
        NSArray *locationArray;
        if (location.size() == 3) {
            locationArray = @[@(location.at(0)), @(location.at(1)), @(location.at(2))];
        } else {
            locationArray = @[];
        }
        
        self.onHoverViro(@{@"source": @(source),
                         @"isHovering":@(isHovering),
                           @"position": locationArray});
    }
}

-(void)onClick:(int)source
    clickState:(VROEventDelegate::ClickState)clickState
 clickLocation:(std::vector<float>)location{
    if (self.onClickViro != nil) {
        NSArray *locationArray;
        if (location.size() == 3) {
            locationArray = @[@(location.at(0)), @(location.at(1)), @(location.at(2))];
        } else {
            locationArray = @[];
        }
        
        self.onClickViro(@{@"source": @(source),
                           @"clickState":@(clickState),
                           @"position": locationArray});
    }
}

- (void)onPinch:(int)source scaleFactor:(float)scale
    pinchState:(VROEventDelegate::PinchState)pinchState {
    if(self.onPinchViro != nil) {
        self.onPinchViro(@{@"source": @(source), @"pinchState":@(pinchState), @"scaleFactor":@(scale)});
    }
}

- (void)onFuse:(int)source {
    if (self.onFuseViro != nil) {
        self.onFuseViro(@{@"source": @(source)});
    }
}

- (void)onDrag:(int)source posX:(float)x posY:(float)y posZ:(float)z {
    if (self.onDragViro != nil) {
        self.onDragViro(@{@"source": @(source),
                          @"dragToPos" : @[@(x), @(y), @(z)]});
    }
}

#pragma mark Physics Implementations
- (void)setScene:(std::shared_ptr<VROScene>)scene {
    [super setScene:scene];

    std::shared_ptr<VROPhysicsBody> body = [self node]->getPhysicsBody();
    if (body){
        scene->getPhysicsWorld()->addPhysicsBody(body);
    }
}

- (void)clearPhysicsBody{
    std::shared_ptr<VROPhysicsBody> body = [self node]->getPhysicsBody();
    if (self.scene && body){
        self.scene->getPhysicsWorld()->removePhysicsBody(body);
    }

    if (body){
        [self node]->clearPhysicsBody();
    }
}

- (std::shared_ptr<VROPhysicsBody>)createPhysicsBody:(VROPhysicsBody::VROPhysicsBodyType) bodyType
                                            withMass:(float)mass
                                           withShape:(std::shared_ptr<VROPhysicsShape>) phsyicsShape{
    std::shared_ptr<VROPhysicsBody> body = [self node]->initPhysicsBody(bodyType,
                                                                        mass,
                                                                        phsyicsShape);
    if (self.scene){
        self.scene->getPhysicsWorld()->addPhysicsBody(body);
    }

    if (_physicsDelegate) {
        body->setPhysicsDelegate(_physicsDelegate);
    } else {
        body->setPhysicsDelegate(nullptr);
    }
    return body;
}

- (void)setPhysicsBody:(NSDictionary *)dictionary{
    // If un-setting the physicsBody, clear it from the node.
    if (!dictionary){
        [self clearPhysicsBody];
        self.physicsDictionary = dictionary;
        return;
    }

    // Else update the current physicsBody with the new properties, recreating
    // the body if needed. Log and return if an error has occured.
    if (![self recreatePhysicsBodyIfNeeded:dictionary]
        || ![self updatePhysicsBodyProperties:dictionary]
        || ![self applyForcesOnBody:dictionary]){
        return;
    }

    // Finally save a copy of the last known set physics properties.
    self.physicsDictionary = dictionary;
}

- (bool)recreatePhysicsBodyIfNeeded:(NSDictionary *)dictionary{
    // Determine if the physics body type has changed
    NSString *nsStringBodyTypeProp = [dictionary objectForKey:@"type"];
    NSString *nsStringBodyTypeCurrent = nullptr;
    if (self.physicsDictionary){
        nsStringBodyTypeCurrent = [self.physicsDictionary objectForKey:@"type"];
    }

    bool hasBodyTypeChanged = nsStringBodyTypeProp != nsStringBodyTypeCurrent;
    if (nsStringBodyTypeProp){
        hasBodyTypeChanged = ![nsStringBodyTypeProp isEqualToString:nsStringBodyTypeCurrent];
    }
    std::string stringBodyType = std::string([nsStringBodyTypeProp UTF8String]);

    // Check if the provided phsyics body type with the given mass is valid.
    std::string errorMsg;
    float mass = [[dictionary objectForKey:@"mass"] floatValue];
    bool isValid = VROPhysicsBody::isValidType(stringBodyType, mass, errorMsg);
    if (!isValid){
        RCTLogError(@"%@", [NSString stringWithUTF8String:errorMsg.c_str()]);
        return false;
    }

    // Determine if the physics shape has changed
    NSDictionary *nsShapeDictionaryProp = [dictionary objectForKey:@"shape"];
    NSDictionary *nsShapeDictionaryCurrent = nullptr;
    if (self.physicsDictionary){
        nsShapeDictionaryCurrent = [self.physicsDictionary objectForKey:@"shape"];
    }

    bool hasBodyShapeChanged = nsShapeDictionaryProp != nsShapeDictionaryCurrent;
    if (nsShapeDictionaryProp){
        hasBodyTypeChanged = ![nsShapeDictionaryProp isEqualToDictionary:nsShapeDictionaryCurrent];
    }

    // Create or update the VROPhysicsBody only if needed
    std::shared_ptr<VROPhysicsBody> body = [self node]->getPhysicsBody();
    if (!body || hasBodyTypeChanged || hasBodyShapeChanged){
        std::shared_ptr<VROPhysicsShape> propPhysicsShape = nullptr;

        // Recreate a physics shape with the latest properties by grabbing
        // the current shapeType (required in JS if providing a physics shape)
        if (nsShapeDictionaryProp){
            NSString *stringShapeName = [nsShapeDictionaryProp objectForKey:@"type"];
            NSArray *shapeParams = [nsShapeDictionaryProp objectForKey:@"params"];
            propPhysicsShape = [VRTNode getPhysicsShape:stringShapeName params:shapeParams];
            if (propPhysicsShape == nullptr){
                return false;
            }
        }

        // Re-create the physics body if the type has changed or if one doesn't exists.
        if (!body || hasBodyTypeChanged){
            // Clean up the existing physicsBody if it exists.
            [self clearPhysicsBody];

            // Create and attach the Physics body to the scene
            VROPhysicsBody::VROPhysicsBodyType propBodyType
                                    = VROPhysicsBody::getBodyTypeForString(stringBodyType);
            body = [self createPhysicsBody:propBodyType withMass:mass withShape:propPhysicsShape];
        } else if (hasBodyShapeChanged){
            body->setPhysicsShape(propPhysicsShape);
        }
    }

    body->setIsSimulated([self shouldAppear]);
    return true;
}

- (bool)updatePhysicsBodyProperties:(NSDictionary *)dictionary {
    std::shared_ptr<VROPhysicsBody> body = [self node]->getPhysicsBody();
    float mass = [[dictionary objectForKey:@"mass"] floatValue];
    std::string stringBodyType = std::string([[dictionary objectForKey:@"type"] UTF8String]);

    NSArray *inertia = [dictionary objectForKey:@"inertia"];
    if (inertia != nil){
        if ([inertia count] != 3) {
            RCTLogError(@"Incorrect parameters provided for inertia, expected: [x, y, z]!");
            return false;
        }

        VROVector3f inertia3f = VROVector3f([[inertia objectAtIndex:1] floatValue],
                                            [[inertia objectAtIndex:2] floatValue],
                                            [[inertia objectAtIndex:3] floatValue]);
        body->setInertia(inertia3f);
    }

    if ([dictionary objectForKey:@"mass"]) {
        std::string errorMsg;
        bool isValid = VROPhysicsBody::isValidType(stringBodyType, mass, errorMsg);
        if (!isValid){
            RCTLogError(@"%@", [NSString stringWithUTF8String:errorMsg.c_str()]);
            return false;
        }
        body->setMass(mass);
    }

    if ([dictionary objectForKey:@"friction"]) {
        float friction = [[dictionary objectForKey:@"friction"] floatValue];
        body->setFriction(friction);
    }

    if ([dictionary objectForKey:@"restitution"]) {
        float restitution = [[dictionary objectForKey:@"restitution"] floatValue];
        body->setRestitution(restitution);
    }

    if ([dictionary objectForKey:@"enabled"]) {
        bool enabled = [[dictionary objectForKey:@"enabled"] boolValue];
        body->setIsSimulated(enabled);
    }

    if ([dictionary objectForKey:@"useGravity"]) {
        bool useGravity = [[dictionary objectForKey:@"useGravity"] boolValue];
        VROPhysicsBody::VROPhysicsBodyType propBodyType
                                    = VROPhysicsBody::getBodyTypeForString(stringBodyType);
        if (propBodyType != VROPhysicsBody::VROPhysicsBodyType::Dynamic && useGravity){
            RCTLogWarn(@"Attempted to set useGravity for non-dynamic phsyics bodies.");
        } else {
            body->setUseGravity(useGravity);
        }
    }
    
    NSArray *velocity = [dictionary objectForKey:@"velocity"];
    if (velocity != nil){
        if ([velocity count] != 3) {
            RCTLogError(@"Incorrect parameters provided for velocity, expected: [x, y, z]!");
            return false;
        }
        
        [self setVelocity:velocity isConstant:YES];
    } else {
        velocity = [NSArray arrayWithObjects:
                    [NSNumber numberWithFloat:0],
                    [NSNumber numberWithFloat:0],
                    [NSNumber numberWithFloat:0],nil];
        
        [self setVelocity:velocity isConstant:YES];
    }
    return true;
}

- (bool)applyForcesOnBody:(NSDictionary *)dictionary{
    NSArray *torqueArray = [dictionary objectForKey:@"torque"];
    NSObject *forceObject = [dictionary objectForKey:@"force"];
    // Check and parse force objects into a dictionary array to iterate over.
    NSArray* forceArray;
    if ([forceObject isKindOfClass:[NSDictionary class]]) {
        forceArray = [[NSArray alloc] initWithObjects:forceObject, nil];
    } else if ([forceObject isKindOfClass:[NSArray class]]) {
        forceArray = (NSArray*)forceObject;
    } else if (forceArray) {
        RCTLogError(@"Invalid force format!");
        return false;
    }
    
    // Deteremine if the applied torque has changed
    NSArray *torqueArrayCurrent = self.physicsDictionary? [self.physicsDictionary objectForKey:@"torque"]: nil;
    bool hasTorqueChanged = torqueArray == torqueArrayCurrent;
    if (torqueArray) {
        hasTorqueChanged = [torqueArray isEqualToArray:torqueArrayCurrent];
    }
    
    // Deteremine if the applied force has changed
    NSObject *forceObjectCurrent = self.physicsDictionary? [self.physicsDictionary objectForKey:@"force"]: nil;
    NSArray* forceArrayCurrent = nil;
    if ([forceObjectCurrent isKindOfClass:[NSDictionary class]]) {
        forceArrayCurrent = [[NSArray alloc] initWithObjects:forceObject, nil];
    } else if ([forceObjectCurrent isKindOfClass:[NSArray class]]) {
        forceArrayCurrent = (NSArray*)forceObject;
    }
    bool hasForceChanged = forceArray == forceArrayCurrent;
    if (forceArray) {
        hasForceChanged = [forceArrayCurrent isEqualToArray:forceArrayCurrent];
    }
    
    // If nothing has changed, return
    if (!hasForceChanged && !hasTorqueChanged) {
        return true;
    }
    
    // Reset forces before applying new ones
    std::shared_ptr<VROPhysicsBody> body = [self node]->getPhysicsBody();
    body->clearForces();
    
    // Apply Toque
    NSArray *torque = [dictionary objectForKey:@"torque"];
    if (torque != nil) {
        if ([torque count] != 3) {
            RCTLogError(@"Incorrect paramters provided for torque, expected: [x, y, z]!");
            return false;
        }
        
        VROVector3f torque3f = VROVector3f([[torque objectAtIndex:0] floatValue],
                                           [[torque objectAtIndex:1] floatValue],
                                           [[torque objectAtIndex:2] floatValue]);        
        body->applyTorque(torque3f);
    }
    
    // Iterate over force array and apply forces to the phsyics body.
    for (int i=0; i < [forceArray count]; i++) {
        if (![forceArray[i] isKindOfClass:[NSDictionary class]]) {
            RCTLogError(@"Invalid format, expected a force format of type dictionary!");
            return false;
        }
        
        // Grab the required power of the force.
        VROVector3f force3f;
        NSArray *power = [forceArray[i] objectForKey:@"power"];
        if (power) {
            if ([power count] != 3) {
                RCTLogError(@"Incorrect parameters provided for force's power, expected: [x, y, z]!");
                return false;
            }
            
            force3f = VROVector3f([[power objectAtIndex:0] floatValue],
                                  [[power objectAtIndex:1] floatValue],
                                  [[power objectAtIndex:2] floatValue]);
        } else {
            RCTLogError(@"Incorrect parameters: missing power of format [x, y, z] for force!");
            return false;
        }
        
        // Grab the optional position of the applied force.
        VROVector3f position3f;
        NSArray *position = [forceArray[i] objectForKey:@"position"];
        if (position) {
            if ([position count] != 3) {
                RCTLogError(@"Incorrect parameters provided for force's position, expected: [x, y, z]!");
                return false;
            }
            
            position3f = VROVector3f([[position objectAtIndex:0] floatValue],
                                     [[position objectAtIndex:1] floatValue],
                                     [[position objectAtIndex:2] floatValue]);
        } else {
            position3f = VROVector3f(0,0,0);
        }
        body->applyForce(force3f, position3f);
    }
    
    return true;
}

-(void)setVelocity:(NSArray*)velocity isConstant:(bool)constant{
    std::shared_ptr<VROPhysicsBody> body = [self node]->getPhysicsBody();
    if (!body) {
        RCTLogError(@"Attempted to set a velocity on a non-physics node");
    }
    VROVector3f velocity3f = VROVector3f([[velocity objectAtIndex:0] floatValue],
                                             [[velocity objectAtIndex:1] floatValue],
                                             [[velocity objectAtIndex:2] floatValue]);
    body->setVelocity(velocity3f, constant);
}

+(std::shared_ptr<VROPhysicsShape>)getPhysicsShape:(NSString *)stringShapeName params:(NSArray *)shapeParams {
    if (!stringShapeName) {
        RCTLogError(@"Provided an invalid physics shape name to the physics body!");
        return nullptr;
    }
    
    // Grab the current shapeParams
    std::vector<float> params = {};
    if (shapeParams) {
        for (int i = 0; i < [shapeParams count]; i ++) {
            float value = [[shapeParams objectAtIndex:i] floatValue];
            params.push_back(value);
        }
    }
    
    // Check if an invalid shape and param was provided.
    std::string errorMsg;
    std::string strShapeName = std::string([stringShapeName UTF8String]);
    bool isValid = VROPhysicsShape::isValidShape(strShapeName, params, errorMsg);
    if (!isValid) {
        RCTLogError(@"%@", [NSString stringWithUTF8String:errorMsg.c_str()]);
        return nullptr;
    }
    
    // Create a VROPhysicsShape
    VROPhysicsShape::VROShapeType propShapeType = VROPhysicsShape::getTypeForString(strShapeName);
    return std::make_shared<VROPhysicsShape>(propShapeType, params);
}

-(void)applyImpulse:(VROVector3f)impulse withOffset:(VROVector3f)offset {
    std::shared_ptr<VROPhysicsBody> body = [self node]->getPhysicsBody();
    if (!body) {
        RCTLogError(@"Attempted to set an impulse force on a non-physics node");
        return;
    }
    body->applyImpulse(impulse, offset);
}


-(void)applyTorqueImpulse:(VROVector3f)torque {
    std::shared_ptr<VROPhysicsBody> body = [self node]->getPhysicsBody();
    if (!body) {
        RCTLogError(@"Attempted to set an impulse force on a non-physics node");
        return;
    }
    body->applyTorqueImpulse(torque);
}

- (void)onCollided:(std::string) bodyKey
         collision:(VROPhysicsBody::VROCollision)collision {

    NSMutableArray *coordinate = [NSMutableArray array];
    [coordinate insertObject:[NSNumber numberWithFloat:collision.collidedPoint.x] atIndex:0];
    [coordinate insertObject:[NSNumber numberWithFloat:collision.collidedPoint.y] atIndex:1];
    [coordinate insertObject:[NSNumber numberWithFloat:collision.collidedPoint.z] atIndex:2];

    NSMutableArray *normal = [NSMutableArray array];
    [normal insertObject:[NSNumber numberWithFloat:collision.collidedNormal.x] atIndex:0];
    [normal insertObject:[NSNumber numberWithFloat:collision.collidedNormal.y] atIndex:1];
    [normal insertObject:[NSNumber numberWithFloat:collision.collidedNormal.z] atIndex:2];

    self.onCollidedViro(@{@"viroTag": @(collision.collidedBodyTag.c_str()), @"collidedPoint":coordinate, @"collidedNormal":normal});
}
@end
