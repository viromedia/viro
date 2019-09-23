//
//  VRTAnimatedComponentManager.m
//  React
//
//  Created by Vik Advani on 2/19/16.
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

#import "VRTAnimatedComponentManager.h"
#import "VRTAnimatedComponent.h"
#import "VRTAnimatedComponentShadowView.h"

@implementation VRTAnimatedComponentManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(animation, NSString);
RCT_EXPORT_VIEW_PROPERTY(loop, BOOL);
RCT_EXPORT_VIEW_PROPERTY(delay, float);
RCT_EXPORT_VIEW_PROPERTY(run, BOOL);
RCT_EXPORT_VIEW_PROPERTY(onAnimationStartViro, RCTDirectEventBlock);
RCT_EXPORT_VIEW_PROPERTY(onAnimationFinishViro, RCTDirectEventBlock);

- (VRTView *)view
{
    return [[VRTAnimatedComponent alloc] initWithBridge:self.bridge];
}

- (VRTShadowView *)shadowView {
    return [VRTAnimatedComponentShadowView new];
}

@end
