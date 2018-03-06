//
//  VRTSceneNavigatorModule.m
//  ViroReact
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <React/RCTUIManager.h>
#import <React/RCTUIManagerUtils.h>

#import "VRTSceneNavigatorModule.h"
#import "VRTSceneNavigator.h"

@implementation VRTSceneNavigatorModule
@synthesize bridge = _bridge;

RCT_EXPORT_MODULE()

- (dispatch_queue_t)methodQueue {
    return RCTGetUIManagerQueue();
}

RCT_EXPORT_METHOD(requestExitVr:(nonnull NSNumber *)sceneNavTag) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        NSLog(@"User Exit VR VRTSceneNav Module RCT_EXPORT_METHOD");
        
        UIView *sceneNavigator = viewRegistry[sceneNavTag];
        
        if (![sceneNavigator isKindOfClass:[VRTSceneNavigator class]]) {
            RCTLogError(@"Invalid view returned when requestingExitVR: expected VRTSceneNavigator, got [%@]", sceneNavigator);
        }
        else {
            NSLog(@"User Exit VR VRTSceneNav Module");
            VRTSceneNavigator *nav = (VRTSceneNavigator *)sceneNavigator;
            [nav userDidRequestExitVR];
        }
    }];
}

RCT_EXPORT_METHOD(recenterTracking:(nonnull NSNumber *)sceneNavTag) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        UIView *sceneNavigator = viewRegistry[sceneNavTag];
        
        if (![sceneNavigator isKindOfClass:[VRTSceneNavigator class]]) {
            RCTLogError(@"Invalid view returned when recenterTracking: expected VRTSceneNavigator, got [%@]", sceneNavigator);
        }
        else {
            NSLog(@"Viro - recentering tracking.");
            VRTSceneNavigator *nav = (VRTSceneNavigator *)sceneNavigator;
            [nav recenterTracking];
        }
    }];
}

@end
