//
//  VRTMaterialVideoManager.m
//  ViroReact
//
//  Created by vik.advani on 3/8/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#import <React/RCTUIManager.h>
#import "VRTMaterialVideoManager.h"
#import "VRTMaterialVideo.h"

@implementation VRTMaterialVideoManager

RCT_EXPORT_MODULE()
RCT_EXPORT_VIEW_PROPERTY(material, NSString)
RCT_EXPORT_VIEW_PROPERTY(paused, BOOL)
RCT_EXPORT_VIEW_PROPERTY(loop, BOOL)
RCT_EXPORT_VIEW_PROPERTY(muted, BOOL)
RCT_EXPORT_VIEW_PROPERTY(volume, float)
RCT_EXPORT_VIEW_PROPERTY(onBufferStartViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onBufferEndViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onFinishViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onErrorViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onUpdateTimeViro, RCTDirectEventBlock)

- (VRTView *)view {
    return [[VRTMaterialVideo alloc] initWithBridge:self.bridge];
}

RCT_EXPORT_METHOD(seekToTime:(nonnull NSNumber *)reactTag time:(float)time)
{
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        VRTView *view = (VRTView *)viewRegistry[reactTag];
        if (![view isKindOfClass:[VRTMaterialVideo class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTVideoSurface, got: %@", view);
        } else {
            VRTMaterialVideo *component = (VRTMaterialVideo *)view;
            [component seekToTime:time];
        }
    }];
}
@end
