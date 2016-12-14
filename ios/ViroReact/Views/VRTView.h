//
//  VRTView.h
//  React
//
//  Created by Vik Advani on 10/21/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#import <ViroKit/ViroKit.h>
#import "VRTEventDelegate.h"
#import "VROComponent.h"
#import "RCTComponent.h"
#import "RCTBridge.h"
#import "UIView+React.h"

typedef NS_ENUM(NSInteger, ViroConstraintType) {
  ViroConstraintTypeNone,
  ViroConstraintTypeBillboard,
};

@interface VRTView : UIView {
    @protected NSMutableArray *_childViews;
}

@property (nonatomic, weak, nullable) id<VRTEventDelegate> delegate;
@property(nullable, nonatomic, weak) VRTView *superview;

@property (nonatomic, readonly, weak) RCTBridge *bridge;

/*
 The following variables are only valid after viewWillAppear is 
 invoked.
 */
@property (nonatomic, readwrite) VRORenderContext *context;
@property (nonatomic, readwrite) std::shared_ptr<VROScene> scene;
@property (nonatomic, readwrite) VRODriver *driver;

- (instancetype)initWithBridge:(RCTBridge *)bridge;

- (void)insertReactSubview:(UIView *)subview atIndex:(NSInteger)atIndex;
- (void)removeReactSubview:(UIView *)subview;
- (NSArray *)reactSubviews;
- (UIView *)reactSuperview;
- (NSNumber *)reactTagAtPoint:(CGPoint)point;

// View/ShadowView is a root view
- (BOOL)isReactRootView;

// Method invoked before view is displayed
- (void)viewWillAppear;

// Method invoked before view is hidden
- (void)viewWillDisappear;

// Returns this view or any child view that matches the hitResult, nil otherwise.
- (VRTView *)findViewHit:(VROHitTestResult)hitResult;

// Returns an array of VRTViews representing the view hierarchy of the view that matches
// the given hitResult starting from the top (this view), nil otherwise.
- (NSArray<VRTView *> *)viewHierarchyOfResult:(VROHitTestResult)hitResult;

// Attempts to execute the given hit event and returns whether or not it was handled
- (BOOL)invokeHitEvent;

// Return true if the view hits the given hit result.
- (BOOL)didViewHit:(VROHitTestResult)hitResult;

// Whether or not this element is hoverable
- (BOOL)hoverable;

@end
