/**
 * Copyright (c) 2016-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import <UIKit/UIKit.h>

#import <yoga/Yoga.h>
#import <React/RCTRootView.h>
#import <React/RCTShadowView.h>
#import "VRTView.h"


@class RCTSparseArray;

typedef NS_ENUM(NSUInteger, VRTUpdateLifecycle) {
  VRTUpdateLifecycleUninitialized = 0,
  VRTUpdateLifecycleComputed,
  VRTUpdateLifecycleDirtied,
};

typedef void (^VRTApplierBlock)(NSDictionary<NSNumber *, VRTView *> *viewRegistry);

/**
 * ShadowView tree mirrors RCT view tree. Every node is highly stateful.
 * 1. A node is in one of three lifecycles: uninitialized, computed, dirtied.
 * 1. RCTBridge may call any of the padding/margin/width/height/top/left setters. A setter would dirty
 *    the node and all of its ancestors.
 * 2. At the end of each Bridge transaction, we call collectUpdatedFrames:widthConstraint:heightConstraint
 *    at the root node to recursively lay out the entire hierarchy.
 * 3. If a node is "computed" and the constraint passed from above is identical to the constraint used to
 *    perform the last computation, we skip laying out the subtree entirely.
 */
@interface VRTShadowView : RCTShadowView
 

@property (nonatomic, weak, readonly) VRTShadowView *superview;


@end

