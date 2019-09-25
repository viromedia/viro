//
//  VRTHUDComponent.m
//  React
//
//  Created by Vik Advani on 1/29/16.
//  Copyright © 2016 Viro Media. All rights reserved.
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

#import "VRTHUDComponent.h"
#import "VRTUtils.h"

@implementation VRTHUDLabel {
    CGFloat _screenWidth;
    CGFloat _screenHeight;
}

@synthesize position = _position;

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _screenWidth = [[UIScreen mainScreen] applicationFrame].size.width;
        _screenHeight =  [[UIScreen mainScreen] applicationFrame].size.height;
        
        self.label =  [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 100, 100)];
        [self.label setTextAlignment:NSTextAlignmentCenter];
        [self.label setFont:[UIFont systemFontOfSize:12]];
        [self.label setBackgroundColor:[UIColor clearColor]];
    }
    
    return self;
}

- (void)setText:(NSString *)text {
    [self.label setText:text];
}

- (void)setColor:(UIColor *)color {
    [self.label setTextColor:color];
}

- (void)setPosition:(NSArray<NSNumber *> *)position {
    _position = [position copy];
    float positionValues[2];
    populateFloatArrayFromNSArray(position, positionValues, 2);
    positionValues[0] = positionValues[0] * _screenWidth;
    positionValues[1] = positionValues[1] * _screenHeight;
    [self.label setCenter:CGPointMake(positionValues[0], positionValues[1])];
}

- (void)setBackgroundColor:(UIColor *)color {
    [self.label setBackgroundColor:[UIColor clearColor]];
}

-(UIView *)getUIView {
    return self.label;
}
@end
