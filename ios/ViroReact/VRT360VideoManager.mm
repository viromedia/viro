//
//  VRT360VideoManager.m
//  React
//
//  Created by Vik Advani on 1/28/16.
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

#import <React/RCTUIManager.h>
#import "VRT360VideoManager.h"
#import "VRT360Video.h"

@implementation VRT360VideoManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(source, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(paused, BOOL)
RCT_EXPORT_VIEW_PROPERTY(loop, BOOL)
RCT_EXPORT_VIEW_PROPERTY(muted, BOOL)
RCT_EXPORT_VIEW_PROPERTY(volume, float)
RCT_EXPORT_VIEW_PROPERTY(rotation, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(onBufferStartViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onBufferEndViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onFinishViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onUpdateTimeViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onErrorViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(stereoMode, NSString)

- (VRT360Video *)view
{
    return [[VRT360Video alloc] initWithBridge:self.bridge];
}

RCT_EXPORT_METHOD(seekToTime:(nonnull NSNumber *)reactTag time:(float)time)
{
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView*> *viewRegistry) {
        VRTView *view = (UIView *)viewRegistry[reactTag];
        if (![view isKindOfClass:[VRT360Video class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTVideoSurface, got: %@", view);
        } else {
            VRT360Video *component = (VRT360Video *)view;
            [component seekToTime:time];
        }
    }];
}

@end
