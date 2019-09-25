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

#import "ViroViewManager.h"

#import <React/RCTBridge.h>
#import <React/RCTConvert.h>
#import <React/RCTEventDispatcher.h>
#import <React/RCTLog.h>
#import <React/RCTShadowView.h>
#import <React/RCTUIManager.h>
#import <React/RCTUIManagerUtils.h>
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

-(BOOL)isRootFlexBoxPanel {
    return NO;
}

#pragma mark - ViroView properties

RCT_EXPORT_VIEW_PROPERTY(transformBehaviors, NSArray<NSString *>)

#pragma mark - View properties


#pragma mark - ShadowView properties

RCT_EXPORT_SHADOW_PROPERTY(top, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(right, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(bottom, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(left, YGValue);

RCT_EXPORT_SHADOW_PROPERTY(width, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(height, YGValue)

RCT_EXPORT_SHADOW_PROPERTY(minWidth, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(maxWidth, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(minHeight, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(maxHeight, YGValue)

RCT_EXPORT_SHADOW_PROPERTY(borderTopWidth, float)
RCT_EXPORT_SHADOW_PROPERTY(borderRightWidth, float)
RCT_EXPORT_SHADOW_PROPERTY(borderBottomWidth, float)
RCT_EXPORT_SHADOW_PROPERTY(borderLeftWidth, float)
RCT_EXPORT_SHADOW_PROPERTY(borderWidth, float)

RCT_EXPORT_SHADOW_PROPERTY(marginTop, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(marginRight, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(marginBottom, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(marginLeft, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(marginVertical, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(marginHorizontal, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(margin, YGValue)

RCT_EXPORT_SHADOW_PROPERTY(paddingTop, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(paddingRight, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(paddingBottom, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(paddingLeft, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(paddingVertical, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(paddingHorizontal, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(padding, YGValue)


RCT_EXPORT_SHADOW_PROPERTY(flex, float)
RCT_EXPORT_SHADOW_PROPERTY(flexGrow, float)
RCT_EXPORT_SHADOW_PROPERTY(flexShrink, float)
RCT_EXPORT_SHADOW_PROPERTY(flexBasis, YGValue)
RCT_EXPORT_SHADOW_PROPERTY(flexDirection, YGFlexDirection)
RCT_EXPORT_SHADOW_PROPERTY(flexWrap, YGWrap)
RCT_EXPORT_SHADOW_PROPERTY(justifyContent, YGJustify)
RCT_EXPORT_SHADOW_PROPERTY(alignItems, YGAlign)
RCT_EXPORT_SHADOW_PROPERTY(alignSelf, YGAlign)
RCT_EXPORT_SHADOW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_SHADOW_PROPERTY(aspectRatio, float)

RCT_EXPORT_SHADOW_PROPERTY(overflow, YGOverflow)

RCT_EXPORT_SHADOW_PROPERTY(onLayout, BOOL)

RCT_EXPORT_SHADOW_PROPERTY(direction, NSNumberArray)

@end
