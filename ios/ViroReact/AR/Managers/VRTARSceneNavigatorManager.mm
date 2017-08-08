//
//  VRTARSceneNavigatorManager.mm
//  ViroReact
//
//  Created by Andy Chu on 6/12/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <React/RCTUIManager.h>
#import <ViroKit/ViroKit.h>
#import "VRTARSceneNavigatorManager.h"
#import "VRTARSceneNavigator.h"

@implementation VRTARSceneNavigatorManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(currentSceneIndex, NSInteger)
RCT_EXPORT_VIEW_PROPERTY(apiKey, NSString)
RCT_EXPORT_VIEW_PROPERTY(onExitViro, RCTDirectEventBlock)

- (VRTARSceneNavigator *)view
{
    return [[VRTARSceneNavigator alloc] initWithBridge:self.bridge];
}

RCT_EXPORT_METHOD(requestExit:(nonnull NSNumber *)reactTag)
{
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        VRTView *view = (VRTView *)viewRegistry[reactTag];
        if (![view isKindOfClass:[VRTARSceneNavigator class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTARSceneNavigator, got: %@", view);
        } else {
            VRTARSceneNavigator *component = (VRTARSceneNavigator *)view;
            [component userDidRequestExitVR];
        }
    }];
}

@end
