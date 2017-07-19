//
//  VRTView.h
//  React
//
//  Created by Vik Advani on 10/21/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#import <ViroKit/ViroKit.h>
#import <React/RCTComponent.h>
#import <React/RCTBridge.h>
#import <React/UIView+React.h>
#import "VRTMaterialManager.h"

typedef NS_ENUM(NSInteger, ViroConstraintType) {
  ViroConstraintTypeNone,
  ViroConstraintTypeBillboard,
};

@interface VRTView : UIView {
    @protected NSMutableArray *_childViews;
}

@property(nullable, nonatomic, weak) VRTView *superview;
@property (nonatomic, readonly, weak) RCTBridge *bridge;

/*
 The following variables are only valid after sceneWillAppear is 
 invoked.
 */
@property (nonatomic, readwrite) VRORenderContext *context;
@property (nonatomic, readwrite) std::shared_ptr<VROScene> scene;
@property (nonatomic, readwrite) std::shared_ptr<VRODriver> driver;
/*
 true/false if it's attached to a parent and its parent has appeared.
 */
@property (nonatomic, assign) BOOL parentHasAppeared;

- (instancetype)initWithBridge:(RCTBridge *)bridge;

- (void)insertReactSubview:(UIView *)subview atIndex:(NSInteger)atIndex;
- (void)removeReactSubview:(UIView *)subview;
- (NSArray *)reactSubviews;
- (UIView *)reactSuperview;
- (NSNumber *)reactTagAtPoint:(CGPoint)point;

// View/ShadowView is a root view
- (BOOL)isReactRootView;

// Whether or not the view should appear
- (BOOL)shouldAppear;

// Methods invoked by parent views to let child views whether or not they
// will appear based on the parent's properties (ie. if parent is visible
// and within the scene graph, but the child may not show up if itself is
// not visible).
- (void)parentDidAppear;
- (void)parentDidDisappear;

// This function handles any logic we need to do due to changes in [self shouldAppear].
- (void)handleAppearanceChange;

// Methods invoked when the scene to which the views belong appears
// and disappears
- (void)sceneWillAppear;
- (void)sceneWillDisappear;

@end
