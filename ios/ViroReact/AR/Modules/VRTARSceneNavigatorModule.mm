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
#import <React/RCTUIManagerUtils.h>
#import "VRTUtils.h"

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
            [component stopVideoRecordingWithHandler:^(BOOL success, NSURL *url,  NSURL *gifPath, NSInteger errorCode) {
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
                    completionHandler:^(BOOL success, NSURL *url,  NSURL *gifPath, NSInteger errorCode) {
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
        }
    }];
}

RCT_EXPORT_METHOD(setWorldOrigin:(nonnull NSNumber *)reactTag
                     worldOrigin:(NSDictionary *)worldOrigin) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        VRTView *view = (VRTView *)viewRegistry[reactTag];
        if (![view isKindOfClass:[VRTARSceneNavigator class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTARSceneNavigator, got: %@", view);
        } else {
            VRTARSceneNavigator *component = (VRTARSceneNavigator *)view;
            VROViewAR *view = (VROViewAR *)[component rootVROView];
            std::shared_ptr<VROARSession> session = [view getARSession];
            
            if (!session) {
                return;
            }
            
            NSArray *posArray = [worldOrigin objectForKey:@"position"];
            NSArray *rotArray = [worldOrigin objectForKey:@"rotation"];
            
            VROMatrix4f originMatrix;
            if (posArray) {
                if ([posArray count] == 3) {
                    float positionValues[3];
                    populateFloatArrayFromNSArray(posArray, positionValues, 3);
                    originMatrix.translate({positionValues[0], positionValues[1], positionValues[2]});
                } else {
                    RCTLogError(@"[Viro] worldOrigin position requires 3 values");
                }
            }
            
            if (rotArray) {
                if ([rotArray count] == 3) {
                    float rotationValues[3];
                    populateFloatArrayFromNSArray(rotArray, rotationValues, 3);
                    originMatrix.rotateX(toRadians(rotationValues[0]));
                    originMatrix.rotateY(toRadians(rotationValues[1]));
                    originMatrix.rotateZ(toRadians(rotationValues[2]));
                } else {
                    RCTLogError(@"[Viro] worldOrigin rotation requires 3 values");
                }
            }
            
            session->setWorldOrigin(originMatrix);
        }
    }];
}


//take 3d position and convert to 2d screen position.
RCT_EXPORT_METHOD(project:(nonnull NSNumber *)reactTag
                  position:(NSArray<NSNumber *> *)position
                  resolve:(RCTPromiseResolveBlock)resolve
                  reject:(RCTPromiseRejectBlock)reject) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        
        VROVector3f pos = VROVector3f([[position objectAtIndex:0] floatValue],
                                      [[position objectAtIndex:1] floatValue],
                                      [[position objectAtIndex:2] floatValue]);

        VRTView *view = (VRTView *)viewRegistry[reactTag];
        if (![view isKindOfClass:[VRTARSceneNavigator class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTSceneNavigator, got: %@", view);
        } else {
            VRTARSceneNavigator *component = (VRTARSceneNavigator *)view;
            VROVector3f projectedPoint = [component projectPoint:pos];
            resolve(@{
                      @"screenPosition" : @[@(projectedPoint.x), @(projectedPoint.y)]});
        }
    }];
}

// take 2d screen position and project into 3d
RCT_EXPORT_METHOD(unproject:(nonnull NSNumber *)reactTag
                  position:(NSArray<NSNumber *> *)position
                  resolve:(RCTPromiseResolveBlock)resolve
                  reject:(RCTPromiseRejectBlock)reject) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {

        VROVector3f pos = VROVector3f([[position objectAtIndex:0] floatValue],
                                      [[position objectAtIndex:1] floatValue],
                                      [[position objectAtIndex:2] floatValue]);

        VRTView *view = (VRTView *)viewRegistry[reactTag];
        if (![view isKindOfClass:[VRTARSceneNavigator class]]) {
            RCTLogError(@"Invalid view returned from registry, expecting VRTSceneNavigator, got: %@", view);
        } else {
            VRTARSceneNavigator *component = (VRTARSceneNavigator *)view;
            VROVector3f projectedPoint = [component unprojectPoint:pos];
            resolve(@{
                      @"position" : @[@(projectedPoint.x), @(projectedPoint.y), @(projectedPoint.z)]});
        }
    }];
}

@end
