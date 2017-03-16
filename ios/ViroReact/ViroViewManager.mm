/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import "ViroViewManager.h"

#import <React/RCTBridge.h>
#import <React/RCTConvert.h>
#import <React/RCTEventDispatcher.h>
#import <React/RCTLog.h>
#import <React/RCTShadowView.h>
#import <React/RCTUIManager.h>
#import <React/RCTUtils.h>
#import <React/RCTView.h>
#import <React/UIView+React.h>

#import "VRTView.h"
#import "VRTShadowView.h"
#import "VRTNode.h"


@implementation RCTConvert(UIAccessibilityTraits)

RCT_MULTI_ENUM_CONVERTER(UIAccessibilityTraits, (@{
  @"none": @(UIAccessibilityTraitNone),
  @"button": @(UIAccessibilityTraitButton),
  @"link": @(UIAccessibilityTraitLink),
  @"header": @(UIAccessibilityTraitHeader),
  @"search": @(UIAccessibilityTraitSearchField),
  @"image": @(UIAccessibilityTraitImage),
  @"selected": @(UIAccessibilityTraitSelected),
  @"plays": @(UIAccessibilityTraitPlaysSound),
  @"key": @(UIAccessibilityTraitKeyboardKey),
  @"text": @(UIAccessibilityTraitStaticText),
  @"summary": @(UIAccessibilityTraitSummaryElement),
  @"disabled": @(UIAccessibilityTraitNotEnabled),
  @"frequentUpdates": @(UIAccessibilityTraitUpdatesFrequently),
  @"startsMedia": @(UIAccessibilityTraitStartsMediaSession),
  @"adjustable": @(UIAccessibilityTraitAdjustable),
  @"allowsDirectInteraction": @(UIAccessibilityTraitAllowsDirectInteraction),
  @"pageTurn": @(UIAccessibilityTraitCausesPageTurn),
}), UIAccessibilityTraitNone, unsignedLongLongValue)

@end

@implementation ViroViewManager


RCT_EXPORT_MODULE()

- (dispatch_queue_t)methodQueue
{
  
  return RCTGetUIManagerQueue();
}

- (VRTView *)viewWithProps:(NSDictionary *)props
{
  return [self view];
}

- (VRTView *)view
{
  return [VRTView new];
}

- (VRTShadowView *)shadowView
{
  return [VRTShadowView new];
}

- (NSArray *)customBubblingEventTypes
{
  return @[

    // Generic events
    @"press",
    @"change",
    @"change",
    @"focus",
    @"blur",
    @"submitEditing",
    @"endEditing",

    // Touch events
    @"touchStart",
    @"touchMove",
    @"touchCancel",
    @"touchEnd",
  ];
}

- (NSArray *)customDirectEventTypes
{
  return @[
    @"layout",
    @"accessibilityTap",
    @"magicTap",
  ];
}

- (NSDictionary *)constantsToExport
{
  return nil;
}

-(BOOL)isRootFlexBoxPanel {
  return NO;
}

#pragma mark - ViroView properties

RCT_EXPORT_VIEW_PROPERTY(transformBehaviors, NSArray<NSString *>)

#pragma mark - View properties


#pragma mark - ShadowView properties

RCT_EXPORT_SHADOW_PROPERTY(backgroundColor, UIColor)

RCT_EXPORT_SHADOW_PROPERTY(top, float)
RCT_EXPORT_SHADOW_PROPERTY(right, float)
RCT_EXPORT_SHADOW_PROPERTY(bottom, float)
RCT_EXPORT_SHADOW_PROPERTY(left, float);

RCT_EXPORT_SHADOW_PROPERTY(width, float)
RCT_EXPORT_SHADOW_PROPERTY(height, float)

RCT_EXPORT_SHADOW_PROPERTY(minWidth, float)
RCT_EXPORT_SHADOW_PROPERTY(maxWidth, float)
RCT_EXPORT_SHADOW_PROPERTY(minHeight, float)
RCT_EXPORT_SHADOW_PROPERTY(maxHeight, float)

RCT_EXPORT_SHADOW_PROPERTY(borderTopWidth, float)
RCT_EXPORT_SHADOW_PROPERTY(borderRightWidth, float)
RCT_EXPORT_SHADOW_PROPERTY(borderBottomWidth, float)
RCT_EXPORT_SHADOW_PROPERTY(borderLeftWidth, float)
RCT_EXPORT_SHADOW_PROPERTY(borderWidth, float)

RCT_EXPORT_SHADOW_PROPERTY(marginTop, float)
RCT_EXPORT_SHADOW_PROPERTY(marginRight, float)
RCT_EXPORT_SHADOW_PROPERTY(marginBottom, float)
RCT_EXPORT_SHADOW_PROPERTY(marginLeft, float)
RCT_EXPORT_SHADOW_PROPERTY(marginVertical, float)
RCT_EXPORT_SHADOW_PROPERTY(marginHorizontal, float)
RCT_EXPORT_SHADOW_PROPERTY(margin, float)

RCT_EXPORT_SHADOW_PROPERTY(paddingTop, float)
RCT_EXPORT_SHADOW_PROPERTY(paddingRight, float)
RCT_EXPORT_SHADOW_PROPERTY(paddingBottom, float)
RCT_EXPORT_SHADOW_PROPERTY(paddingLeft, float)
RCT_EXPORT_SHADOW_PROPERTY(paddingVertical, float)
RCT_EXPORT_SHADOW_PROPERTY(paddingHorizontal, float)
RCT_EXPORT_SHADOW_PROPERTY(padding, float)


RCT_EXPORT_SHADOW_PROPERTY(flex, float)
RCT_EXPORT_SHADOW_PROPERTY(flexGrow, float)
RCT_EXPORT_SHADOW_PROPERTY(flexShrink, float)
RCT_EXPORT_SHADOW_PROPERTY(flexBasis, float)
RCT_EXPORT_SHADOW_PROPERTY(flexDirection, YGFlexDirection)
RCT_EXPORT_SHADOW_PROPERTY(flexWrap, YGWrap)
RCT_EXPORT_SHADOW_PROPERTY(justifyContent, YGJustify)
RCT_EXPORT_SHADOW_PROPERTY(alignItems, YGAlign)
RCT_EXPORT_SHADOW_PROPERTY(alignSelf, YGAlign)
RCT_EXPORT_SHADOW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_SHADOW_PROPERTY(aspectRatio, float)

RCT_EXPORT_SHADOW_PROPERTY(overflow, YGOverflow)

RCT_EXPORT_SHADOW_PROPERTY(onLayout, BOOL)

@end
