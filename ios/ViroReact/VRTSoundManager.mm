//
//  VRTSoundManager.m
//  ViroReact
//
//  Created by Andy Chu on 1/27/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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
