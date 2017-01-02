//
//  VROLabelManager.m
//  React
//
//  Created by Vik Advani on 1/13/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VROTextManager.h"
#import "VRTLabelShadowView.h"
#import "VRTLabel.h"

@implementation VROTextManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(text, NSString)
RCT_EXPORT_VIEW_PROPERTY(color, UIColor)
RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray);
RCT_EXPORT_VIEW_PROPERTY(backgroundColor, UIColor);

RCT_EXPORT_VIEW_PROPERTY(fontFamily, NSString)
RCT_EXPORT_VIEW_PROPERTY(fontSize, CGFloat)
RCT_EXPORT_VIEW_PROPERTY(fontWeight, NSString)
RCT_EXPORT_VIEW_PROPERTY(fontStyle, NSString)
RCT_EXPORT_VIEW_PROPERTY(letterSpacing, CGFloat)
RCT_EXPORT_VIEW_PROPERTY(lineHeight, CGFloat)
RCT_EXPORT_VIEW_PROPERTY(maxLines, NSUInteger)
RCT_EXPORT_VIEW_PROPERTY(visible, BOOL)
RCT_EXPORT_VIEW_PROPERTY(width, NSUInteger)
RCT_EXPORT_VIEW_PROPERTY(shadowOffset, CGSize)
RCT_EXPORT_VIEW_PROPERTY(textAlign, NSTextAlignment)
RCT_EXPORT_VIEW_PROPERTY(textDecorationStyle, NSUnderlineStyle)
RCT_EXPORT_VIEW_PROPERTY(textDecorationColor, UIColor)
RCT_EXPORT_VIEW_PROPERTY(textDecorationLine, RCTTextDecorationLineType)
RCT_EXPORT_VIEW_PROPERTY(writingDirection, NSWritingDirection)
//RCT_EXPORT_VIEW_PROPERTY(allowFontScaling, BOOL)
RCT_EXPORT_VIEW_PROPERTY(opacity, CGFloat)

RCT_EXPORT_VIEW_PROPERTY(transformBehaviors, NSArray<NSString *>)

RCT_EXPORT_VIEW_PROPERTY(onTapViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onGazeViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(canTap, BOOL)
RCT_EXPORT_VIEW_PROPERTY(canGaze, BOOL)

- (VRTView *)view
{
  return [[VRTLabel alloc] initWithBridge:self.bridge];
}

- (VRTLabelShadowView *)shadowView
{
  return [VRTLabelShadowView new];
}


@end
