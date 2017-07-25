//
//  VRTARNavigatorManager.m
//  ViroReact
//
//  Created by Andy Chu on 6/12/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <React/RCTUIManager.h>
#import <ViroKit/ViroKit.h>
#import "VRTARNavigatorManager.h"
#import "VRTARNavigator.h"

@implementation VRTARNavigatorManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(currentSceneIndex, NSInteger)
RCT_EXPORT_VIEW_PROPERTY(apiKey, NSString)
RCT_EXPORT_VIEW_PROPERTY(onExitViro, RCTDirectEventBlock)

- (VRTARNavigator *)view
{
    return [[VRTARNavigator alloc] initWithBridge:self.bridge];
}

RCT_EXPORT_METHOD(requestExit:(nonnull NSNumber *)reactTag)
{
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        VRTView *view = (VRTView *)viewRegistry[reactTag];
        if (![view isKindOfClass:[VRTARNavigator class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTARNavigator, got: %@", view);
        } else {
            VRTARNavigator *component = (VRTARNavigator *)view;
            [component userDidRequestExitVR];
        }
    }];
}

@end
