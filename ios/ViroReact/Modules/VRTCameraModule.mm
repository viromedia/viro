//
//  VRTCameraModule.m
//  ViroReact
//
//  Created by Raj Advani on 1/26/17.
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
#import "VRTCameraModule.h"
#import "VRTScene.h"
#import <React/RCTUIManagerUtils.h>

@implementation VRTCameraModule

@synthesize bridge = _bridge;

RCT_EXPORT_MODULE()

- (dispatch_queue_t)methodQueue {
    return RCTGetUIManagerQueue();
}

RCT_EXPORT_METHOD(getCameraOrientation:(nonnull NSNumber *)reactTag
                  resolve:(RCTPromiseResolveBlock)resolve
                  reject:(RCTPromiseRejectBlock)reject) {
    
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        UIView *view = viewRegistry[reactTag];
        if (![view isKindOfClass:[VRTScene class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTScene, got: %@", view);
        } else {
            resolve(((VRTScene *)view).cameraOrientation);
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
