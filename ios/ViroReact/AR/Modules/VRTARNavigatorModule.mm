//
//  VRTARNavigatorModule.m
//  ViroReact
//
//  Created by Andy Chu on 7/25/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <React/RCTUIManager.h>
#import "VRTARNavigatorModule.h"
#import "VRTARNavigator.h"

@implementation VRTARNavigatorModule
@synthesize bridge = _bridge;

RCT_EXPORT_MODULE()

- (dispatch_queue_t)methodQueue {
    return RCTGetUIManagerQueue();
}


RCT_EXPORT_METHOD(startVideoRecording:(nonnull NSNumber *)reactTag
                             fileName:(NSString *)fileName
                     saveToCameraRoll:(BOOL)saveToCameraRoll) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        VRTView *view = (VRTView *)viewRegistry[reactTag];
        if (![view isKindOfClass:[VRTARNavigator class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTARNavigator, got: %@", view);
        } else {
            VRTARNavigator *component = (VRTARNavigator *)view;
            [component startVideoRecording:fileName saveToCameraRoll:saveToCameraRoll];
        }
    }];
}

RCT_EXPORT_METHOD(stopVideoRecording:(nonnull NSNumber *)reactTag
                  resolve:(RCTPromiseResolveBlock)resolve
                  reject:(RCTPromiseRejectBlock)reject) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        VRTView *view = (VRTView *)viewRegistry[reactTag];
        if (![view isKindOfClass:[VRTARNavigator class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTARNavigator, got: %@", view);
        } else {
            VRTARNavigator *component = (VRTARNavigator *)view;
            [component stopVideoRecordingWithHandler:^(NSURL *url) {
                resolve([url path]);
            }];
        }
    }];
}

RCT_EXPORT_METHOD(takeScreenshot:(nonnull NSNumber *)reactTag
                        fileName:(NSString *)fileName
                saveToCameraRoll:(BOOL)saveToCameraRoll
                         resolve:(RCTPromiseResolveBlock)resolve
                          reject:(RCTPromiseRejectBlock)reject) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        VRTView *view = (VRTView *)viewRegistry[reactTag];
        if (![view isKindOfClass:[VRTARNavigator class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTARNavigator, got: %@", view);
        } else {
            VRTARNavigator *component = (VRTARNavigator *)view;
            [component takeScreenshot:fileName saveToCameraRoll:saveToCameraRoll completionHandler:^(NSURL *url) {
                resolve([url path]);
            }];
        }
    }];
}

@end
