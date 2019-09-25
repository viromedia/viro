//
//  VRTSoundManager.m
//  ViroReact
//
//  Created by Andy Chu on 1/27/17.
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

#import <React/RCTUIManager.h>
#import "VRTSound.h"
#import "VRTSoundManager.h"

@implementation VRTSoundManager

RCT_EXPORT_MODULE()
RCT_EXPORT_VIEW_PROPERTY(source, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(paused, BOOL)
RCT_EXPORT_VIEW_PROPERTY(volume, float)
RCT_EXPORT_VIEW_PROPERTY(muted, BOOL)
RCT_EXPORT_VIEW_PROPERTY(loop, BOOL)
RCT_EXPORT_VIEW_PROPERTY(onFinishViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onErrorViro, RCTDirectEventBlock)

- (VRTView *)view
{
    return [[VRTSound alloc] initWithBridge:self.bridge];
}

RCT_EXPORT_METHOD(seekToTime:(nonnull NSNumber *)reactTag time:(NSInteger)time)
{
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        VRTView *view = (VRTView *)viewRegistry[reactTag];
        if (![view isKindOfClass:[VRTSound class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTSound, got: %@", view);
        } else {
            VRTSound *component = (VRTSound *)view;
            [component seekToTime:time];
        }
    }];
}

@end

@implementation VRTSoundFieldManager

RCT_EXPORT_MODULE()
RCT_EXPORT_VIEW_PROPERTY(source, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(paused, BOOL)
RCT_EXPORT_VIEW_PROPERTY(volume, float)
RCT_EXPORT_VIEW_PROPERTY(muted, BOOL)
RCT_EXPORT_VIEW_PROPERTY(loop, BOOL)
RCT_EXPORT_VIEW_PROPERTY(rotation, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(onFinishViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onErrorViro, RCTDirectEventBlock)

- (VRTView *)view
{
    return [[VRTSoundField alloc] initWithBridge:self.bridge];
}

RCT_EXPORT_METHOD(seekToTime:(nonnull NSNumber *)reactTag time:(NSInteger)time)
{
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        VRTView *view = (VRTView *)viewRegistry[reactTag];
        if (![view isKindOfClass:[VRTSoundField class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTSoundField, got: %@", view);
        } else {
            VRTSoundField *component = (VRTSoundField *)view;
            [component seekToTime:time];
        }
    }];
}

@end

@implementation VRTSpatialSoundManager

RCT_EXPORT_MODULE()
RCT_EXPORT_VIEW_PROPERTY(source, NSDictionary)
RCT_EXPORT_VIEW_PROPERTY(paused, BOOL)
RCT_EXPORT_VIEW_PROPERTY(volume, float)
RCT_EXPORT_VIEW_PROPERTY(muted, BOOL)
RCT_EXPORT_VIEW_PROPERTY(loop, BOOL)
RCT_EXPORT_VIEW_PROPERTY(position, NSNumberArray)
RCT_EXPORT_VIEW_PROPERTY(minDistance, float)
RCT_EXPORT_VIEW_PROPERTY(maxDistance, float)
RCT_EXPORT_VIEW_PROPERTY(rolloffModel, NSString)
RCT_EXPORT_VIEW_PROPERTY(onFinishViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onErrorViro, RCTDirectEventBlock)

- (VRTView *)view
{
    return [[VRTSpatialSoundWrapper alloc] initWithBridge:self.bridge];
}

RCT_EXPORT_METHOD(seekToTime:(nonnull NSNumber *)reactTag time:(NSInteger)time)
{
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        VRTView *view = (VRTView *)viewRegistry[reactTag];
        if (![view isKindOfClass:[VRTSpatialSound class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTSpatialSound, got: %@", view);
        } else {
            VRTSpatialSound *component = (VRTSpatialSound *)view;
            [component seekToTime:time];
        }
    }];
}

@end
