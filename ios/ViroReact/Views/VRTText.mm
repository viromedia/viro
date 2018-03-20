//
//  VRTText.m
//  React
//
//  Created by Vik Advani on 1/12/16.
//  Copyright © 2017 Viro Media. All rights reserved.
//
#import <CoreText/CoreText.h>
#import <React/RCTConvert.h>
#import <React/RCTLog.h>
#import <React/RCTFont.h>
#import "VRTText.h"
#import "VRTUtils.h"
#import "VRTFlexView.h"

NSString *const VRTLabelReactTagAttributeName = @"ReactTagAttributeName";

@implementation VRTText {
    std::shared_ptr<VROText> _vroText;
    NSString *_text;
    NSString *_defaultFont;
    float _ratio;
    NSMutableAttributedString *_cachedMutableAttributedString;
    BOOL _boundsCalculated;
    CGRect _frame;
    BOOL _textNeedsUpdate;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    
    if (self) {
        _boundsCalculated = false;
        _width = 1.0;
        _height = 1.0;
        _ratio = 1.0;
        _fontSize = 18;
        _defaultFont = @"";
        _maxLines = 0;
        _fontWeight = VROFontWeight::Regular;
        _fontStyle = VROFontStyle::Normal;
        _textClipMode = VROTextClipMode::ClipToBounds;
        _textLineBreakMode = VROLineBreakMode::WordWrap;
        _textAlign = VROTextHorizontalAlignment::Left;
        _textAlignVertical = VROTextVerticalAlignment::Top;
        _textNeedsUpdate = NO;
    }
    return self;
}

- (void)setText:(NSString *)text {
    _text = text;
    _textNeedsUpdate = YES;
}

- (void)setWidth:(float)width {
    _width = width;
    _textNeedsUpdate = YES;
}

- (void)setHeight:(float)height {
    _height = height;
    _textNeedsUpdate = YES;
}

- (void)setColor:(UIColor *)color {
    _color = color;
    _textNeedsUpdate = YES;
}

- (void)setFontFamily:(NSString *)fontFamily {
    _fontFamily = [fontFamily copy];
    _textNeedsUpdate = YES;
}

- (void)setFontSize:(CGFloat)fontSize {
    _fontSize = fontSize;
    _textNeedsUpdate = YES;
}

- (void)setFontWeight:(VROFontWeight)fontWeight {
    _fontWeight = fontWeight;
    _textNeedsUpdate = YES;
}

- (void)setFontStyle:(VROFontStyle)fontStyle {
    _fontStyle = fontStyle;
    _textNeedsUpdate = YES;
}

- (void)setMaxLines:(NSUInteger)maxLines {
    _maxLines = maxLines;
    _textNeedsUpdate = YES;
}

- (void)setTextAlign:(VROTextHorizontalAlignment)textAlign {
    _textAlign = textAlign;
    _textNeedsUpdate = YES;
}

- (void)setTextAlignVertical:(VROTextVerticalAlignment)textAlignVertical {
    _textAlignVertical = textAlignVertical;
    _textNeedsUpdate = YES;
}

- (void)setTextLineBreakMode:(VROLineBreakMode)textLineBreakMode {
    _textLineBreakMode = textLineBreakMode;
    _textNeedsUpdate = YES;
}

- (void)setTextClipMode:(VROTextClipMode)textClipMode {
    _textClipMode = textClipMode;
    _textNeedsUpdate = YES;
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    if (self.driver && _textNeedsUpdate) {
        [self updateLabel];
        _textNeedsUpdate = NO;
    }
}

- (void)updateLabel {
    NSString *fontFamilyNS = ([self.fontFamily length]) ? self.fontFamily: _defaultFont;
    
    std::string fontFamily = std::string([fontFamilyNS UTF8String]);
    int fontSize = (int)self.fontSize;
    std::shared_ptr<VROTypeface> typeface = self.driver->newTypeface(fontFamily, fontSize, self.fontStyle, self.fontWeight);
    
    if (_text != nil && [_text length] != 0) {
        NSStringEncoding encoding = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE);
        NSData *textData = [_text dataUsingEncoding:encoding];

        std::wstring text = std::wstring((wchar_t *) [textData bytes], [textData length] / sizeof(wchar_t));
        VROVector4f colorVector(1.0f, 1.0f, 1.0f, 1.0f);

        if (self.color != nil) {
            CGFloat red,green,blue, alpha;
            [self.color getRed:&red green:&green blue:&blue alpha:&alpha];
            colorVector.set(red, green, blue, alpha);
        }
        _vroText = VROText::createText(text, typeface, colorVector, _width, _height,
                                       self.textAlign, self.textAlignVertical,
                                       self.textLineBreakMode, self.textClipMode, (int) self.maxLines);

        [self node]->setGeometry(_vroText);
    } else {
        [self node]->setGeometry(nil);
    }
}

// Method invoked before being added to scene, meant to overridden by subclasses.
- (void)sceneWillAppear {
    [super sceneWillAppear];
    [self updateLabel];
}

@end

@implementation RCTConvert (VRTText)
+ (VROTextHorizontalAlignment)VROTextHorizontalAlignment:(id)json {
    if (![json isKindOfClass:[NSString class]]) {
        RCTLogError(@"Error setting horizontal alignment: string required, received: %@", json);
        return VROTextHorizontalAlignment::Left;
    }
    
    NSString *value = (NSString *)json;
    if ([value caseInsensitiveCompare:@"Left"] == NSOrderedSame ) {
        return VROTextHorizontalAlignment::Left;
    } else if([value caseInsensitiveCompare:@"Center"] == NSOrderedSame) {
        return VROTextHorizontalAlignment::Center;
    } else if([value caseInsensitiveCompare:@"Right"] == NSOrderedSame) {
        return VROTextHorizontalAlignment::Right;
    }
    return VROTextHorizontalAlignment::Left;
}

+ (VROTextVerticalAlignment)VROTextVerticalAlignment:(id)json {
    if (![json isKindOfClass:[NSString class]]) {
        RCTLogError(@"Error setting vertical alignment: string required, received: %@", json);
        return VROTextVerticalAlignment::Top;
    }
    
    NSString *value = (NSString *)json;
    if([value caseInsensitiveCompare:@"Top"] == NSOrderedSame) {
        return VROTextVerticalAlignment::Top;
    } else if([value caseInsensitiveCompare:@"Center"] == NSOrderedSame) {
        return VROTextVerticalAlignment::Center;
    } else if([value caseInsensitiveCompare:@"Bottom"] == NSOrderedSame) {
        return VROTextVerticalAlignment::Bottom;
    }
    return VROTextVerticalAlignment::Top;
}

+ (VROLineBreakMode)VROLineBreakMode:(id)json {
    if (![json isKindOfClass:[NSString class]]) {
        RCTLogError(@"Error setting line break mode: string required, received: %@", json);
        return VROLineBreakMode::None;
    }
    
    NSString *value = (NSString *)json;
    if([value caseInsensitiveCompare:@"WordWrap"] == NSOrderedSame) {
        return VROLineBreakMode::WordWrap;
    } else if([value caseInsensitiveCompare:@"CharWrap"] == NSOrderedSame) {
        return VROLineBreakMode::CharWrap;
    } else if([value caseInsensitiveCompare:@"Justify"] == NSOrderedSame) {
        return VROLineBreakMode::Justify;
    } else if([value caseInsensitiveCompare:@"None"] == NSOrderedSame) {
        return VROLineBreakMode::None;
    }
    return VROLineBreakMode::None;
}

+ (VROTextClipMode)VROTextClipMode:(id)json {
    if (![json isKindOfClass:[NSString class]]) {
        RCTLogError(@"Error setting text clip mode: string required, received: %@", json);
        return VROTextClipMode::None;
    }
    
    NSString *value = (NSString *)json;
    if ([value caseInsensitiveCompare:@"None"] == NSOrderedSame) {
        return VROTextClipMode::None;
    }
    else if ([value caseInsensitiveCompare:@"ClipToBounds"] == NSOrderedSame) {
        return VROTextClipMode::ClipToBounds;
    }
    return VROTextClipMode::None;
}

+ (VROFontStyle)VROFontStyle:(id)json {
    if (![json isKindOfClass:[NSString class]]) {
        RCTLogError(@"Error setting font style: string required, received: %@", json);
        return VROFontStyle::Normal;
    }
    
    NSString *fontStyle = (NSString *)json;
    if (fontStyle == nil || fontStyle.length == 0) {
        return VROFontStyle::Normal;
    } else if ([@"italic" caseInsensitiveCompare:fontStyle] == 0) {
        return VROFontStyle::Italic;
    } else {
        NSLog(@"Unrecognized font style [%@]", fontStyle);
        return VROFontStyle::Normal;
    }
}

+ (VROFontWeight)VROFontWeight:(id)json {
    if (![json isKindOfClass:[NSString class]]) {
        RCTLogError(@"Error setting font weight: string required, received: %@", json);
        return VROFontWeight::Regular;
    }
    
    NSString *fontWeight = (NSString *)json;
    if (fontWeight == nil || fontWeight.length == 0) {
        return VROFontWeight::Regular;
    } else if ([@"bold" caseInsensitiveCompare:fontWeight] == 0) {
        return VROFontWeight::Bold;
    } else if ([@"normal" caseInsensitiveCompare:fontWeight] == 0) {
        return VROFontWeight::Regular;
    } else if ([@"100" caseInsensitiveCompare:fontWeight] == 0) {
        return VROFontWeight::UltraLight;
    } else if ([@"200" caseInsensitiveCompare:fontWeight] == 0) {
        return VROFontWeight::Thin;
    } else if ([@"300" caseInsensitiveCompare:fontWeight] == 0) {
        return VROFontWeight::Light;
    } else if ([@"400" caseInsensitiveCompare:fontWeight] == 0) {
        return VROFontWeight::Regular;
    } else if ([@"500" caseInsensitiveCompare:fontWeight] == 0) {
        return VROFontWeight::Medium;
    } else if ([@"600" caseInsensitiveCompare:fontWeight] == 0) {
        return VROFontWeight::Semibold;
    } else if ([@"700" caseInsensitiveCompare:fontWeight] == 0) {
        return VROFontWeight::Bold;
    } else if ([@"800" caseInsensitiveCompare:fontWeight] == 0) {
        return VROFontWeight::Heavy;
    } else if ([@"900" caseInsensitiveCompare:fontWeight] == 0) {
        return VROFontWeight::ExtraBlack;
    } else {
        NSLog(@"Unrecognized font weight [%@]", fontWeight);
        return VROFontWeight::Regular;
    }
}

@end
