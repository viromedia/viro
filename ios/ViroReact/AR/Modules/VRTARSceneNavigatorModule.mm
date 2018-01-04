//
//  VRTARSceneNavigatorModule.m
//  ViroReact
//
//  Created by Andy Chu on 7/25/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <React/RCTUIManager.h>
#import "VRTARSceneNavigatorModule.h"
#import "VRTARSceneNavigator.h"

@implementation VRTARSceneNavigatorModule
@synthesize bridge = _bridge;

static NSString *const kVRTRecordingKeySuccess = @"success";
static NSString *const kVRTRecordingKeyUrl = @"url";
static NSString *const kVRTRecordingKeyErrorCode = @"errorCode";

RCT_EXPORT_MODULE()

- (dispatch_queue_t)methodQueue {
    return RCTGetUIManagerQueue();
}

RCT_EXPORT_METHOD(startVideoRecording:(nonnull NSNumber *)reactTag
                             fileName:(NSString *)fileName
                     saveToCameraRoll:(BOOL)saveToCameraRoll
                              callback:(RCTResponseSenderBlock)callback) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        VRTView *view = (VRTView *)viewRegistry[reactTag];
        if (![view isKindOfClass:[VRTARSceneNavigator class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTARSceneNavigator, got: %@", view);
        } else {
            VRTARSceneNavigator *component = (VRTARSceneNavigator *)view;
            [component startVideoRecording:fileName saveToCameraRoll:saveToCameraRoll onError:callback];
        }
    }];
}

RCT_EXPORT_METHOD(stopVideoRecording:(nonnull NSNumber *)reactTag
                             resolve:(RCTPromiseResolveBlock)resolve
                              reject:(RCTPromiseRejectBlock)reject) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        VRTView *view = (VRTView *)viewRegistry[reactTag];
        if (![view isKindOfClass:[VRTARSceneNavigator class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTARSceneNavigator, got: %@", view);
        } else {
            VRTARSceneNavigator *component = (VRTARSceneNavigator *)view;
            [component stopVideoRecordingWithHandler:^(BOOL success, NSURL *url, NSInteger errorCode) {
                NSMutableDictionary *toReturn = [NSMutableDictionary new];
                [toReturn setObject:@(success) forKey:kVRTRecordingKeySuccess];
                if (url) [toReturn setObject:[url path] forKey:kVRTRecordingKeyUrl];
                [toReturn setObject:@(errorCode) forKey:kVRTRecordingKeyErrorCode];
                resolve(toReturn);
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
        if (![view isKindOfClass:[VRTARSceneNavigator class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTARSceneNavigator, got: %@", view);
        } else {
            VRTARSceneNavigator *component = (VRTARSceneNavigator *)view;
            [component takeScreenshot:fileName
                     saveToCameraRoll:saveToCameraRoll
                    completionHandler:^(BOOL success, NSURL *url, NSInteger errorCode) {
                NSMutableDictionary *toReturn = [NSMutableDictionary new];
                [toReturn setObject:@(success) forKey:kVRTRecordingKeySuccess];
                if (url) [toReturn setObject:[url path] forKey:kVRTRecordingKeyUrl];
                [toReturn setObject:@(errorCode) forKey:kVRTRecordingKeyErrorCode];
                resolve(toReturn);
            }];
        }
    }];
}

RCT_EXPORT_METHOD(resetARSession:(nonnull NSNumber *)reactTag
                  resetTracking:(BOOL)resetTracking
                  removeAnchors:(BOOL)removeAnchors) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        VRTView *view = (VRTView *)viewRegistry[reactTag];
        if (![view isKindOfClass:[VRTARSceneNavigator class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTARSceneNavigator, got: %@", view);
        } else {
            VRTARSceneNavigator *component = (VRTARSceneNavigator *)view;
            VROViewAR *view = (VROViewAR *)[component rootVROView];
            view.getARSession->resetSession(resetTracking, removeAnchors);
            NSLog(@"kirby reset! %@", removeAnchors ? @"removing!" : @"not removing!");
        }
    }];
}

@end
