//
//  VRTNode.h
//  React
//
//  Created by Raj Advani on 8/24/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <React/RCTComponent.h>
#import <React/RCTBridge.h>
#import "VRTView.h"

extern const int k2DPointsPerSpatialUnit;

@interface VRTNode : VRTView<VROEventDelegateProtocol, VROPhysicsBodyDelegateProtocol, VROTransformDelegateProtocol>

- (instancetype)initWithBridge:(RCTBridge *)bridge;

@property (readwrite, nonatomic) std::shared_ptr<VRONode> node;
@property (readwrite, nonatomic) std::shared_ptr<VROEventDelegateiOS> eventDelegate;
@property (readwrite, nonatomic) std::shared_ptr<VROPhysicsBodyDelegateiOS> physicsDelegate;
@property (readwrite, nonatomic) std::shared_ptr<VROTransformDelegateiOS> transformDelegate;

// Rendering properties
@property (nonatomic, copy, nullable) NSArray<NSString *> *materials;
@property (nonatomic, copy, nullable) NSArray<NSNumber *> *position;
@property (nonatomic, copy, nullable) NSArray<NSNumber *> *rotation;
@property (nonatomic, copy, nullable) NSArray<NSNumber *> *scale;
@property (nonatomic, copy, nullable) NSArray<NSNumber *> *rotationPivot;
@property (nonatomic, copy, nullable) NSArray<NSNumber *> *scalePivot;
@property (nonatomic, assign) float opacity;
@property (nonatomic, copy, nullable) NSArray<NSString *> *transformBehaviors;
@property (nonatomic, assign) BOOL visible;

// Event properties
@property (nonatomic, copy, nullable) RCTDirectEventBlock onHoverViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onClickViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onFuseViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onDragViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onPinchViro;
@property (nonatomic, assign) BOOL canHover;
@property (nonatomic, assign) BOOL canClick;
@property (nonatomic, assign) BOOL canFuse;
@property (nonatomic, assign) BOOL canDrag;
@property (nonatomic, assign) BOOL canPinch;
@property (nonatomic, assign) BOOL highAccuracyGaze;
@property (nonatomic, assign) float timeToFuse;

// Transform Update properties:
@property (nonatomic, copy, nullable) RCTDirectEventBlock onNativeTransformDelegateViro;
@property (nonatomic, assign) BOOL hasTransformDelegate;

// Physics properties
@property (nonatomic, copy, nullable) RCTDirectEventBlock onCollidedViro;
@property (nonatomic, copy) NSDictionary *physicsDictionary;
- (void)applyImpulse:(VROVector3f)impulse withOffset:(VROVector3f)offset;
- (void)applyTorqueImpulse:(VROVector3f)torque;
- (void)setVelocity:(NSArray*)velocity isConstant:(bool)constant;
- (void)onCollided:(std::string) bodyKey collision:(VROPhysicsBody::VROCollision) collision;
- (void)clearPhysicsBody;
+ (std::shared_ptr<VROPhysicsShape>)getPhysicsShape:(NSString *)stringShapeName params:(NSArray *)shapeParams;
@property (nonatomic, assign) BOOL canCollide;

// Used for Flexbox enabled components, no-op for non flexbox related components.
@property(nonatomic) CGPoint position2DFlex;
@property(nonatomic) CGPoint centerPoint2DFlex;;
@property(nonatomic) CGRect bounds2DFlex;

- (void)reactSetFrame:(CGRect)frame;
- (BOOL)isRootFlexboxView;
- (CGPoint)fudgeFlexboxScaleX:(float)width3d Y:(float)height3d;

// Apply materials stored in _materials property to current node geometry if geometry is not null.
- (void)applyMaterials;

// VROEventDelegateProtocol
- (void) onHover:(int)source isHovering:(bool)isHovering hoverLocation:(std::vector<float>)location;
- (void) onClick:(int)source clickState:(VROEventDelegate::ClickState)clickState clickLocation:(std::vector<float>)location;
- (void) onTouch:(int)source touchState:(VROEventDelegate::TouchState)clickState xPos:(float)x yPos:(float)y;
- (void) onFuse:(int)source;
- (void) onDrag:(int)source posX:(float)x posY:(float)y posZ:(float)z;
- (void) onPinch:(int)source scaleFactor:(float)scale pinchState:(VROEventDelegate::PinchState)pinchState;

// Override for AR nodes
- (std::shared_ptr<VRONode>)createVroNode;
@end
