//
//  VRTView.h
//  React
//
//  Created by Vik Advani on 10/21/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#import <ViroKit/ViroKit.h>
#import "VROComponent.h"
#import "RCTComponent.h"
#import "RCTBridge.h"
#import "UIView+React.h"
#import "VROMaterialManager.h"

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
@end
