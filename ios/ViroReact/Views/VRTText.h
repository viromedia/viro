//
//  VRTText.h
//  React
//
//  Created by Vik Advani on 1/12/16.
//  Copyright © 2017 Viro Media. All rights reserved.
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

#import <Foundation/Foundation.h>
#import <React/RCTTextDecorationLineType.h>
#import <React/RCTConvert.h>
#import "VRTControl.h"


@interface VRTText : VRTControl

- (instancetype)initWithBridge:(RCTBridge *)bridge;

@property (nonatomic, strong) UIColor *color;
@property (nonatomic, copy) NSString *fontFamily;
@property (nonatomic, assign) CGFloat fontSize;
@property (nonatomic, assign) VROFontWeight fontWeight;
@property (nonatomic, assign) VROFontStyle fontStyle;
@property (nonatomic, assign) NSUInteger maxLines;
@property (nonatomic, assign) VROTextHorizontalAlignment textAlign;
@property (nonatomic, assign) VROTextVerticalAlignment textAlignVertical;
@property (nonatomic, assign) VROLineBreakMode textLineBreakMode;
@property (nonatomic, assign) VROTextClipMode textClipMode;
@property (nonatomic, assign) float width;
@property (nonatomic, assign) float height;
@property (nonatomic, assign) float extrusionDepth;
@property (nonatomic, assign) VROTextOuterStroke outerStrokeType;
@property (nonatomic, assign) int outerStrokeWidth;
@property (nonatomic, strong) UIColor *outerStrokeColor;
@property (readwrite, nonatomic) NSDictionary *outerStroke;

@end

@interface RCTConvert (VRTText)

+ (VROTextHorizontalAlignment)VROTextHorizontalAlignment:(id)json;
+ (VROTextVerticalAlignment)VROTextVerticalAlignment:(id)json;
+ (VROLineBreakMode)VROLineBreakMode:(id)json;
+ (VROTextClipMode)VROTextClipMode:(id)json;
+ (VROFontStyle)VROFontStyle:(id)json;
+ (VROFontWeight)VROFontWeight:(id)json;

@end
