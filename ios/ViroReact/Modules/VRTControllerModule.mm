//
//  VRTControllerModule.m
//  ViroReact
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <React/RCTUIManager.h>
#import "VRTControllerModule.h"
#import "VRTController.h"

@implementation VRTControllerModule
@synthesize bridge = _bridge;

RCT_EXPORT_MODULE()

- (dispatch_queue_t)methodQueue {
    return RCTGetUIManagerQueue();
}

RCT_EXPORT_METHOD(getForwardVectorAsync:(nonnull NSNumber *)viewTag
                  resolve:(RCTPromiseResolveBlock)resolve
                  reject:(RCTPromiseRejectBlock)reject) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        UIView *controllerView = viewRegistry[viewTag];
        
        if (![controllerView isKindOfClass:[VRTController class]]) {
            RCTLogError(@"Invalid view returned when getForwardVectorAsync: expected VRTController, got [%@]", controllerView);
        }
        else {
            VRTController *controller = (VRTController *)controllerView;
            resolve([controller getControllerForwardVector]);
        }
    }];
}

@end
