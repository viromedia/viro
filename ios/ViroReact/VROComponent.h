//
//  VROComponent.h
//  React
//
//  Created by Vik Advani on 11/3/15.
//  Copyright © 2016 Viro Media. All rights reserved.
//
#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
//  NOTE: This is copied from RTCComponent.m When updating to latest React, ensure:
//    1. React has same method signatures, change VROComponent to match them.
//    2. Add new methods if they are added to RTCComponent.m

@protocol VROComponent<NSObject>

@property (nonatomic, copy) NSNumber *reactTag;

- (void)insertReactSubview:(id<VROComponent>)subview atIndex:(NSInteger)atIndex;
- (void)removeReactSubview:(id<VROComponent>)subview;
- (NSArray *)reactSubviews;
- (id<VROComponent>)reactSuperview;
- (NSNumber *)reactTagAtPoint:(CGPoint)point;

// View/ShadowView is a root view
- (BOOL)isReactRootView;

@optional

/**
 * Called each time props have been set.
 * Not all props have to be set - React can set only changed ones.
 * @param changedProps String names of all set props.
 */
- (void)didSetProps:(NSArray<NSString *> *)changedProps;

/**
 * Called each time subviews have been updated
 */
- (void)didUpdateReactSubviews;

// TODO: Deprecate this
// This method is called after layout has been performed for all views known
// to the RCTViewManager. It is only called on UIViews, not shadow views.
- (void)reactBridgeDidFinishTransaction;

@end


