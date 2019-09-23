//
//  VRTNode.h
//  React
//
//  Created by Raj Advani on 8/24/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#import <Foundation/Foundation.h>
#import <React/RCTComponent.h>
#import <React/RCTBridge.h>
#import "VRTView.h"
#import "VRTManagedAnimation.h"

extern const int k2DPointsPerSpatialUnit;

@class VRTNodeAnimation;
@class VRTAnimationManager;

@interface VRTNodeAnimation : VRTManagedAnimation

@property (readwrite, nonatomic) NSString *animationName;
@property (readwrite, nonatomic) VRTAnimationManager *animationManager;

- (std::shared_ptr<VROExecutableAnimation>)loadAnimation;

@end

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
@property (nonatomic, assign) int renderingOrder;
@property (nonatomic, assign) BOOL visible;

// Event properties
@property (nonatomic, copy, nullable) RCTDirectEventBlock onHoverViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onClickViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onFuseViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onDragViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onPinchViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onRotateViro;
@property (nonatomic, assign) BOOL canHover;
@property (nonatomic, assign) BOOL canClick;
@property (nonatomic, assign) BOOL canFuse;
@property (nonatomic, assign) BOOL canDrag;
@property (nonatomic, assign) BOOL canPinch;
@property (nonatomic, assign) BOOL canRotate;
@property (nonatomic, assign) BOOL highAccuracyEvents;
@property (nonatomic, assign) float timeToFuse;
@property (nonatomic, copy, nullable) NSString *dragType;
@property (nonatomic, copy) NSDictionary *dragPlane;

/*
 Whether or not we actually ignore event handling is determined by "and"-ing this component's
 ignoreEventHandling with shouldIgnoreHandling, which is the parent's resolved ignoreEventHandling.
 */
@property (nonatomic, assign) BOOL ignoreEventHandling;
@property (nonatomic, assign) BOOL shouldIgnoreEventHandling; // parent's resolved ignoreEventHandling

// Animation object
@property (nonatomic) VRTNodeAnimation *nodeAnimation;
// Animation definition
@property (readwrite, nonatomic) NSDictionary *animation;
@property (nonatomic, copy) RCTDirectEventBlock onAnimationStartViro;
@property (nonatomic, copy) RCTDirectEventBlock onAnimationFinishViro;

// Shadow/Light properties
@property (nonatomic, assign) int lightReceivingBitMask;
@property (nonatomic, assign) int shadowCastingBitMask;

// Transform Update properties:
@property (nonatomic, copy, nullable) RCTDirectEventBlock onNativeTransformDelegateViro;
@property (nonatomic, assign) BOOL hasTransformDelegate;

// Physics properties
@property (nonatomic, copy, nullable) RCTDirectEventBlock onCollisionViro;
@property (nonatomic, copy) NSDictionary *physicsDictionary;
- (void)applyImpulse:(VROVector3f)impulse withOffset:(VROVector3f)offset;
- (void)applyTorqueImpulse:(VROVector3f)torque;
- (void)setVelocity:(NSArray*)velocity isConstant:(bool)constant;
- (void)onCollided:(std::string) bodyKey collision:(VROPhysicsBody::VROCollision) collision;
- (void)clearPhysicsBody;
+ (std::shared_ptr<VROPhysicsShape>)getPhysicsShape:(NSString *)stringShapeName params:(NSArray *)shapeParams;
@property (nonatomic, assign) BOOL canCollide;
@property (nonatomic, assign) BOOL physicsEnabled;

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
- (void) onHover:(int)source node:(std::shared_ptr<VRONode>)node isHovering:(bool)isHovering hoverLocation:(std::vector<float>)location;
- (void) onClick:(int)source node:(std::shared_ptr<VRONode>)node clickState:(VROEventDelegate::ClickState)clickState clickLocation:(std::vector<float>)location;
- (void) onTouch:(int)source node:(std::shared_ptr<VRONode>)node touchState:(VROEventDelegate::TouchState)clickState xPos:(float)x yPos:(float)y;
- (void) onFuse:(int)source node:(std::shared_ptr<VRONode>)node;
- (void) onDrag:(int)source node:(std::shared_ptr<VRONode>)node posX:(float)x posY:(float)y posZ:(float)z;
- (void) onPinch:(int)source node:(std::shared_ptr<VRONode>)node scaleFactor:(float)scale pinchState:(VROEventDelegate::PinchState)pinchState;
- (void) onRotate:(int)source node:(std::shared_ptr<VRONode>)node rotationRadians:(float)rotationRadians rotateState:(VROEventDelegate::RotateState)rotateState;
- (void) onCameraARHitTest:(int)source results:(std::vector<VROARHitTestResult>)results;

// Override for AR nodes
- (std::shared_ptr<VRONode>)createVroNode;
@end
