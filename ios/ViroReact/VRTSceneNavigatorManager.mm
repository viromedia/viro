//
//  VRTSceneNavigatorManager.m
//  React
//
//  Created by Vik Advani on 3/11/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <React/RCTUIManager.h>
#import "VRTSceneNavigatorManager.h"
#import "VRTSceneNavigator.h"


@implementation VRTSceneNavigatorManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(currentSceneIndex, NSInteger)
RCT_EXPORT_VIEW_PROPERTY(vrModeEnabled, BOOL)
RCT_EXPORT_VIEW_PROPERTY(apiKey, NSString)
RCT_EXPORT_VIEW_PROPERTY(onExitViro, RCTDirectEventBlock)

- (VRTSceneNavigator *)view
{
    return [[VRTSceneNavigator alloc] initWithBridge:self.bridge];
}

RCT_EXPORT_METHOD(requestExit:(nonnull NSNumber *)reactTag)
{
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        VRTView *view = (VRTView *)viewRegistry[reactTag];
        if (![view isKindOfClass:[VRTSceneNavigator class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTSceneNavigator, got: %@", view);
        } else {
            VRTSceneNavigator *component = (VRTSceneNavigator *)view;
            [component userDidRequestExitVR];
        }
    }];
    
}

@end
