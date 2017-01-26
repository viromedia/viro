//
//  VRTCameraModule.m
//  ViroReact
//
//  Created by Raj Advani on 1/26/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import "VRTCameraModule.h"
#import "RCTUIManager.h"
#import "VRTScene.h"

@implementation VRTCameraModule

@synthesize bridge = _bridge;

RCT_EXPORT_MODULE()

- (dispatch_queue_t)methodQueue {
    return RCTGetUIManagerQueue();
}

RCT_EXPORT_METHOD(getCameraPosition:(nonnull NSNumber *)reactTag
                  resolve:(RCTPromiseResolveBlock)resolve
                  reject:(RCTPromiseRejectBlock)reject) {
    
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        UIView *view = viewRegistry[reactTag];
        if (![view isKindOfClass:[VRTScene class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTScene, got: %@", view);
        } else {
            resolve(((VRTScene *)view).cameraPosition);
        }
    }];
}

RCT_EXPORT_METHOD(setSceneCamera:(nonnull NSNumber *)sceneTag camera:(nonnull NSNumber *)cameraTag) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        UIView *cameraView = viewRegistry[cameraTag];
        UIView *sceneView = viewRegistry[sceneTag];
        
        if (![cameraView isKindOfClass:[VRTCamera class]]) {
            RCTLogError(@"Invalid view returned when setting camera: expected VRTCamera, got [%@]", cameraView);
        }
        else if (![sceneView isKindOfClass:[VRTScene class]]) {
            RCTLogError(@"Invalid view returned when setting camera: expected VRTScene, got [%@]", sceneView);
        }
        else {
            VRTCamera *camera = (VRTCamera *)cameraView;
            VRTScene *scene = (VRTScene *)sceneView;
            [scene setCamera:camera];
        }
    }];
}

RCT_EXPORT_METHOD(removeSceneCamera:(nonnull NSNumber *)sceneTag camera:(nonnull NSNumber *)cameraTag) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        UIView *cameraView = viewRegistry[cameraTag];
        UIView *sceneView = viewRegistry[sceneTag];
        
        if (![cameraView isKindOfClass:[VRTCamera class]]) {
            RCTLogError(@"Invalid view returned when removing camera: expected VRTCamera, got [%@]", cameraView);
        }
        else if (![sceneView isKindOfClass:[VRTScene class]]) {
            RCTLogError(@"Invalid view returned when removing camera: expected VRTScene, got [%@]", sceneView);
        }
        else {
            VRTCamera *camera = (VRTCamera *)cameraView;
            VRTScene *scene = (VRTScene *)sceneView;
            [scene removeCamera:camera];
        }
    }];
}

@end
