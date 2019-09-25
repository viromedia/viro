//
//  VRTView.h
//  React
//
//  Created by Vik Advani on 10/21/15.
//  Copyright © 2015 Viro Media. All rights reserved.
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
// did appear based on the parent's properties (ie. if parent is visible
// and within the scene graph, but the child may not show up if itself is
// not visible). Children override these methods to perform actions based
// on the view appearing or disappearing.
- (void)parentDidAppear;
- (void)parentDidDisappear;

// This function handles any logic we need to do due to changes in [self shouldAppear].
- (void)handleAppearanceChange;

// Methods invoked when the scene to which the views belong appears
// and disappears
- (void)sceneWillAppear;
- (void)sceneWillDisappear;

@end
