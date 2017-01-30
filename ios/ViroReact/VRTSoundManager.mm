//
//  VRTSoundManager.m
//  ViroReact
//
//  Created by Andy Chu on 1/27/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

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

- (VRTView *)view
{
    return [[VRTSound alloc] initWithBridge:self.bridge];
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

- (VRTView *)view
{
    return [[VRTSoundField alloc] initWithBridge:self.bridge];
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
RCT_EXPORT_VIEW_PROPERTY(onFinishViro, RCTDirectEventBlock)

- (VRTView *)view
{
    return [[VRTSpatialSoundWrapper alloc] initWithBridge:self.bridge];
}

@end
