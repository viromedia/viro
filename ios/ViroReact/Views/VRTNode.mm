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
#import "VRTPortal.h"
#import "VRT360Image.h"
#import "VRT3DObject.h"
#import "VRTAnimatedComponent.h"
#import "VRTMaterialManager.h"
#import "VRTManagedAnimation.h"
#import "VRTAnimationManager.h"

const int k2DPointsPerSpatialUnit = 1000;
const double kTransformDelegateDistanceFilter = 0.01;

#pragma mark - Node Animation Class

@implementation VRTNodeAnimation

- (std::shared_ptr<VROExecutableAnimation>)loadAnimation {
    if (self.animationName != nil) {
        std::shared_ptr<VROExecutableAnimation> animation = [self.animationManager animationForName:self.animationName];
        if (animation) {
            return animation->copy();
        }
        else {
            return nullptr;
        }
    }
    else {
        return nullptr;
    }
}

- (void)setAnimationName:(NSString *)animationName {
    _animationName = [animationName copy];
}

@end

#pragma mark - Node Class

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
        _lightReceivingBitMask = 1;
        _shadowCastingBitMask = 1;
        _shouldIgnoreEventHandling = NO; // default is NO
        _ignoreEventHandling = NO; // default is NO
        
        _nodeAnimation = [[VRTNodeAnimation alloc] init];
        _nodeAnimation.animationManager = [bridge animationManager];
        _nodeAnimation.node = _node;
        
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
    } else if ([child isKindOfClass:[VRTPortal class]]) {
        // Ignore, this is only handled by VRTPortal
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
    else if ([vroView isKindOfClass:[VRTPortal class]]) {
        // Ignore, this is only handled by VRTPortal
    }
    else if([vroView isKindOfClass:[VRT360Image class]]) {
        VRTNode *nodeView = (VRTNode *)vroView;
        nodeView.node->getParentPortal()->removeBackground();
        nodeView.node->removeFromParentNode();
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
        body->setIsSimulated([self shouldAppear] && body->getIsSimulated());
    }
    [self node]->setHidden(![self shouldAppear]);
    [super handleAppearanceChange];
}

- (void)setVisible:(BOOL)visible {
    _visible = visible;
    [self handleAppearanceChange];
}

- (void)setViroTag:(NSString *)tag {
    std::string nodeTag;
    if (tag) {
        nodeTag = std::string([tag UTF8String]);
    }
    [self node]->setTag(nodeTag);
}

- (void)setContext:(VRORenderContext *)context {
    [super setContext:context];
    [self updateVideoTextures];
}

- (void)setDriver:(std::shared_ptr<VRODriver>)driver {
    [super setDriver:driver];
    [self updateVideoTextures];
}

- (void)updateVideoTextures {
    if(self.driver != nil && self.context != nil) {
        VRTMaterialManager *materialManager = [self.bridge moduleForClass:[VRTMaterialManager class]];
        std::vector<std::shared_ptr<VROMaterial>>::iterator it;
        if(self.node->getGeometry() == NULL) {
            return;
        }
        std::vector<std::shared_ptr<VROMaterial>> materialsVec = self.node->getGeometry()->getMaterials();

        for(it = materialsVec.begin(); it != materialsVec.end(); it++) {
            std::string name = it->get()->getName();
            NSString *materialNameNSString = [NSString stringWithCString:name.c_str()
                                                                encoding:NSASCIIStringEncoding];
            BOOL isVideoMaterial = [materialManager isVideoMaterial:materialNameNSString];
            if(isVideoMaterial) {
                [materialManager loadVideoTextureForMaterial:materialNameNSString driver:self.driver context:self.context];
            }
        }
    }
}

#pragma mark - Transforms

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

#pragma mark - Materials
- (void)setMaterials:(NSArray<NSString *> *)materials {
    _materials = materials;
    [self applyMaterials];
}

// Apply materials to the underlying geometry if materials were explicitly set
// via the materials prop
- (void)applyMaterials {
    if (!self.node) {
        return;
    }
    
    std::shared_ptr<VROGeometry> geometry = self.node->getGeometry();
    if (!geometry) {
        return;
    }
    
    if (!self.materials) {
        // If materials were removed from object, then reset the materials array.
        std::vector<std::shared_ptr<VROMaterial>> tempMaterials;
        tempMaterials.push_back(std::make_shared<VROMaterial>());
        geometry->setMaterials(tempMaterials);
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
    [self updateVideoTextures];
}

#pragma mark - Animation

- (void)setAnimation:(NSDictionary *)animation {
    [self.nodeAnimation parseFromDictionary:animation];
    self.nodeAnimation.animationName = [animation objectForKey:@"name"];
    [self.nodeAnimation updateAnimation];
}

- (void)setOnAnimationStartViro:(RCTDirectEventBlock)onAnimationStartViro {
    self.nodeAnimation.onStart = onAnimationStartViro;
}

- (void)setOnAnimationFinishViro:(RCTDirectEventBlock)onAnimationFinishViro {
    self.nodeAnimation.onFinish = onAnimationFinishViro;
}

#pragma mark - Flexbox

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

#pragma mark - Events

- (void)setHighAccuracyGaze:(BOOL)enabled{
    _highAccuracyGaze = enabled;
    [self node]->setHighAccuracyGaze(enabled);
}

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

- (void)setCanRotate:(BOOL)canRotate {
    _canRotate = canRotate;
    self.eventDelegate->setEnabledEvent(VROEventDelegate::EventAction::OnRotate, canRotate);
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

- (void)setDragType:(NSString *)dragType {
    if ([dragType caseInsensitiveCompare:@"FixedDistance"] == NSOrderedSame) {
        _node->setDragType(VRODragType::FixedDistance);
    } else if ([dragType caseInsensitiveCompare:@"FixedToWorld"] == NSOrderedSame) {
        _node->setDragType(VRODragType::FixedToWorld);
    } else {
        RCTLogError(@"Received unknown drag type: %@", dragType);
    }
}

- (void)setIgnoreEventHandling:(BOOL)ignoreEventHandling {
    _ignoreEventHandling = ignoreEventHandling;
    [self resolveIgnoreEventHandling];
}

- (void)setShouldIgnoreEventHandling:(BOOL)ignoreEventHandling {
    _shouldIgnoreEventHandling = ignoreEventHandling;
    [self resolveIgnoreEventHandling];
}

- (void)resolveIgnoreEventHandling {
    BOOL resolvedIgnoreEventHandling = _ignoreEventHandling || _shouldIgnoreEventHandling;
    [self node]->setIgnoreEventHandling(resolvedIgnoreEventHandling);
    for (VRTNode *child : [self reactSubviews]) {
        child.shouldIgnoreEventHandling = resolvedIgnoreEventHandling;
    }
}

- (void)setLightReceivingBitMask:(int)lightReceivingBitMask {
    _lightReceivingBitMask = lightReceivingBitMask;
    _node->setLightReceivingBitMask(lightReceivingBitMask);
}

- (void)setShadowCastingBitMask:(int)shadowCastingBitMask {
    _shadowCastingBitMask = shadowCastingBitMask;
    _node->setShadowCastingBitMask(shadowCastingBitMask);
}

-(void)onHover:(int)source node:(std::shared_ptr<VRONode>)node
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

-(void)onClick:(int)source node:(std::shared_ptr<VRONode>)node clickState:(VROEventDelegate::ClickState)clickState
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

- (void)onPinch:(int)source node:(std::shared_ptr<VRONode>)node scaleFactor:(float)scale
     pinchState:(VROEventDelegate::PinchState)pinchState {
    if(self.onPinchViro != nil) {
        self.onPinchViro(@{@"source": @(source), @"pinchState":@(pinchState), @"scaleFactor":@(scale)});
    }
}

- (void)onRotate:(int)source node:(std::shared_ptr<VRONode>)node rotationRadians:(float)rotation
     rotateState:(VROEventDelegate::RotateState)rotateState {
    if(self.onRotateViro != nil) {
        // convert to degrees from radians
        float degreesRotation = toDegrees(rotation);
        self.onRotateViro(@{@"source": @(source), @"rotateState":@(rotateState), @"rotationFactor":@(degreesRotation)});
    }
}

- (void)onFuse:(int)source node:(std::shared_ptr<VRONode>)node {
    if (self.onFuseViro != nil) {
        self.onFuseViro(@{@"source": @(source)});
    }
}

- (void)onDrag:(int)source node:(std::shared_ptr<VRONode>)node posX:(float)x posY:(float)y posZ:(float)z {
    if (self.onDragViro != nil) {
        self.onDragViro(@{@"source": @(source),
                          @"dragToPos" : @[@(x), @(y), @(z)]});
    }
}

- (void)onCameraARHitTest:(std::vector<VROARHitTestResult>)results {
    //no-op base class for this event.
}

- (void)onARPointCloudUpdated:(std::shared_ptr<VROARPointCloud>)pointCloud {
    //no-op base class for this event.
}

#pragma mark - Physics Implementations

- (void)setScene:(std::shared_ptr<VROScene>)scene {
    [super setScene:scene];
}

- (void)clearPhysicsBody {
    std::shared_ptr<VROPhysicsBody> body = [self node]->getPhysicsBody();
    if (body) {
        [self node]->clearPhysicsBody();
    }
}

- (std::shared_ptr<VROPhysicsBody>)createPhysicsBody:(VROPhysicsBody::VROPhysicsBodyType)bodyType
                                            withMass:(float)mass
                                           withShape:(std::shared_ptr<VROPhysicsShape>) physicsShape {
    std::shared_ptr<VROPhysicsBody> body = [self node]->initPhysicsBody(bodyType, mass, physicsShape);
    if (_physicsDelegate) {
        body->setPhysicsDelegate(_physicsDelegate);
    } else {
        body->setPhysicsDelegate(nullptr);
    }
    return body;
}

- (void)setPhysicsBody:(NSDictionary *)dictionary {
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
    bool hasTorqueChanged = torqueArray != torqueArrayCurrent;
    if (torqueArray) {
        hasTorqueChanged = ![torqueArray isEqualToArray:torqueArrayCurrent];
    }

    // Deteremine if the applied force has changed
    NSObject *forceObjectCurrent = self.physicsDictionary? [self.physicsDictionary objectForKey:@"force"]: nil;
    NSArray* forceArrayCurrent = nil;
    if ([forceObjectCurrent isKindOfClass:[NSDictionary class]]) {
        forceArrayCurrent = [[NSArray alloc] initWithObjects:forceObjectCurrent, nil];
    } else if ([forceObjectCurrent isKindOfClass:[NSArray class]]) {
        forceArrayCurrent = (NSArray*)forceObjectCurrent;
    }
    bool hasForceChanged = forceArray != forceArrayCurrent;
    if (forceArray) {
        hasForceChanged = ![forceArray isEqualToArray:forceArrayCurrent];
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
        
        // Grab the required value of the force.
        VROVector3f force3f;
        NSArray *value = [forceArray[i] objectForKey:@"value"];
        if (value) {
            if ([value count] != 3) {
                RCTLogError(@"Incorrect parameters provided for force's value, expected: [x, y, z]!");
                return false;
            }
            
            force3f = VROVector3f([[value objectAtIndex:0] floatValue],
                                  [[value objectAtIndex:1] floatValue],
                                  [[value objectAtIndex:2] floatValue]);
        } else {
            RCTLogError(@"Incorrect parameters: missing value of format [x, y, z] for force!");
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
    
    self.onCollisionViro(@{@"viroTag": @(collision.collidedBodyTag.c_str()), @"collidedPoint":coordinate, @"collidedNormal":normal});
}
@end
