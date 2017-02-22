/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import <UIKit/UIKit.h>


#import <React/RCTBridgeModule.h>
#import <React/RCTEventDispatcher.h>
#import <React/RCTViewManager.h>
#import <React/RCTConvert.h>
#import <React/RCTLog.h>

@class RCTBridge;
@class RCTEventDispatcher;
@class RCTShadowView;
@class RCTSparseArray;
@class ViroUIManager;
@class VRTView;
@class VRTShadowView;

typedef void (^ViroViewManagerUIBlock)(ViroUIManager *uiManager, NSDictionary<NSNumber *, VRTView *> *viewRegistry);

@interface ViroViewManager : RCTViewManager <RCTBridgeModule>

-(BOOL)isRootFlexBoxPanel;

@end
