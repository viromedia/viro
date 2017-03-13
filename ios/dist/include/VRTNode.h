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

@interface VRTNode : VRTView<VROEventDelegateProtocol>

- (instancetype)initWithBridge:(RCTBridge *)bridge;

@property (readwrite, nonatomic) std::shared_ptr<VRONode> node;
@property (readwrite, nonatomic) std::shared_ptr<VROEventDelegateiOS> eventDelegate;

// Rendering properties
@property (nonatomic, copy, nullable) NSArray<NSString *> *materials;
@property (nonatomic, copy, nullable) NSArray<NSNumber *> *position;
@property (nonatomic, copy, nullable) NSArray<NSNumber *> *rotation;
@property (nonatomic, copy, nullable) NSArray<NSNumber *> *scale;
@property (nonatomic, assign) float opacity;
@property (nonatomic, copy, nullable) NSArray<NSString *> *transformBehaviors;
@property (nonatomic, assign) BOOL visible;

// Event properties
@property (nonatomic, copy, nullable) RCTDirectEventBlock onHoverViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onClickViro;
@property (nonatomic, assign) BOOL canHover;
@property (nonatomic, assign) BOOL canClick;
@property (nonatomic, assign) BOOL highAccuracyGaze;

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
- (void) onHover:(int)source isHovering:(bool)isHovering;
- (void) onClick:(int)source clickState:(VROEventDelegate::ClickState)clickState;
- (void) onTouch:(int)source touchState:(VROEventDelegate::TouchState)clickState xPos:(float)x yPos:(float)y;
@end
