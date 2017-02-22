//
//  VRTText.h
//  React
//
//  Created by Vik Advani on 1/12/16.
//  Copyright © 2017 Viro Media. All rights reserved.
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
@property (nonatomic, assign) NSUInteger maxLines;
@property (nonatomic, assign) VROTextHorizontalAlignment textAlign;
@property (nonatomic, assign) VROTextVerticalAlignment textAlignVertical;
@property (nonatomic, assign) VROLineBreakMode textLineBreakMode;
@property (nonatomic, assign) VROTextClipMode textClipMode;

@property (nonatomic, assign) float width;
@property (nonatomic, assign) float height;


@end

@interface RCTConvert (VRTText)

+ (VROTextHorizontalAlignment)VROTextHorizontalAlignment:(id)json;
+ (VROTextVerticalAlignment)VROTextVerticalAlignment:(id)json;
+ (VROLineBreakMode)VROLineBreakMode:(id)json;
+ (VROTextClipMode)VROTextClipMode:(id)json;

@end
