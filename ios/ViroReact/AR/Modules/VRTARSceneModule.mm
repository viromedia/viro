//
//  VRTARSceneModule.mm
//  ViroReact
//
//  Created by Andy Chu on 8/9/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <React/RCTUIManager.h>
#import "VRTARScene.h"
#import "VRTARSceneModule.h"
#import "VRTARSceneNavigator.h"
#import "VRTARHitTestUtil.h"

@implementation VRTARSceneModule
@synthesize bridge = _bridge;

RCT_EXPORT_MODULE()

- (dispatch_queue_t)methodQueue {
    return RCTGetUIManagerQueue();
}

RCT_EXPORT_METHOD(performARHitTestWithRay:(nonnull NSNumber *)viewTag
                  ray:(NSArray *)ray
                  resolver:(RCTPromiseResolveBlock)resolve
                  rejecter:(RCTPromiseRejectBlock)reject) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        UIView *sceneView = viewRegistry[viewTag];
        if (![sceneView isKindOfClass:[VRTARScene class]]) {
            RCTLogError(@"Invalid view returned when calling performARHitTestWithRay: expected VRTARScene, got [%@]", sceneView);
        } else if ([ray count] != 3) {
            RCTLogError(@"Invalid Ray provided for performARHitTestWithRay!");
        } else {
            VRTARScene *scene = (VRTARScene *)sceneView;
            UIView *superview = [scene superview];
            if (superview && [superview isKindOfClass:[VRTARSceneNavigator class]]) {
                VRTARSceneNavigator *navigator = (VRTARSceneNavigator *)superview;
                if ([navigator rootVROView]) {
                    VROViewAR *view = (VROViewAR *)[navigator rootVROView];
                    VROVector3f rayVector = VROVector3f([[ray objectAtIndex:0] floatValue], [[ray objectAtIndex:1] floatValue], [[ray objectAtIndex:2] floatValue]);
                    std::vector<VROARHitTestResult> results = [view performARHitTest:rayVector];
                    NSMutableArray *returnArray = [[NSMutableArray alloc] initWithCapacity:results.size()];
                    for (VROARHitTestResult result : results) {
                        [returnArray addObject:[VRTARHitTestUtil dictForARHitResult:result]];
                    }
                    resolve(returnArray);
                }
            }
        }
    }];
}

RCT_EXPORT_METHOD(performARHitTestWithPosition:(nonnull NSNumber *)viewTag
                  position:(NSArray *)position
                  resolver:(RCTPromiseResolveBlock)resolve
                  rejecter:(RCTPromiseRejectBlock)reject) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        UIView *sceneView = viewRegistry[viewTag];
        if (![sceneView isKindOfClass:[VRTARScene class]]) {
            RCTLogError(@"Invalid view returned when calling performARHitTestWithPosition: expected VRTARScene, got [%@]", sceneView);
        } else if ([position count] != 3) {
            RCTLogError(@"Invalid Position provided for performARHitTestWithPosition!");
        } else {
            VRTARScene *scene = (VRTARScene *)sceneView;
            UIView *superview = [scene superview];
            if (superview && [superview isKindOfClass:[VRTARSceneNavigator class]]) {
                VRTARSceneNavigator *navigator = (VRTARSceneNavigator *)superview;
                if ([navigator rootVROView]) {
                    VROViewAR *view = (VROViewAR *)[navigator rootVROView];
                    VROVector3f targetPosition = VROVector3f([[position objectAtIndex:0] floatValue],
                                                             [[position objectAtIndex:1] floatValue],
                                                             [[position objectAtIndex:2] floatValue]);
                    NSArray *cameraOrientation = [scene cameraOrientation];
                    VROVector3f cameraPosition = VROVector3f([[cameraOrientation objectAtIndex:0] floatValue],
                                                             [[cameraOrientation objectAtIndex:1] floatValue],
                                                             [[cameraOrientation objectAtIndex:2] floatValue]);
                    std::vector<VROARHitTestResult> results = [view performARHitTest:(targetPosition - cameraPosition)];
                    NSMutableArray *returnArray = [[NSMutableArray alloc] initWithCapacity:results.size()];
                    for (VROARHitTestResult result : results) {
                        [returnArray addObject:[VRTARHitTestUtil dictForARHitResult:result]];
                    }
                    resolve(returnArray);
                }
            }
        }
    }];
}

RCT_EXPORT_METHOD(performARHitTestWithPoint:(nonnull NSNumber *)viewTag
                  x:(int)x
                  y:(int)y
                  resolver:(RCTPromiseResolveBlock)resolve
                  rejecter:(RCTPromiseRejectBlock)reject) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        UIView *sceneView = viewRegistry[viewTag];
        if (![sceneView isKindOfClass:[VRTARScene class]]) {
            RCTLogError(@"Invalid view returned when calling performARHitTestWithPoint: expected VRTARScene, got [%@]", sceneView);
        } else {
            VRTARScene *scene = (VRTARScene *)sceneView;
            UIView *superview = [scene superview];
            if (superview && [superview isKindOfClass:[VRTARSceneNavigator class]]) {
                VRTARSceneNavigator *navigator = (VRTARSceneNavigator *)superview;
                if ([navigator rootVROView]) {
                    VROViewAR *view = (VROViewAR *)[navigator rootVROView];
                    std::vector<VROARHitTestResult> results = [view performARHitTestWithPoint:x y:y];
                    NSMutableArray *returnArray = [[NSMutableArray alloc] initWithCapacity:results.size()];
                    for (VROARHitTestResult result : results) {
                        [returnArray addObject:[VRTARHitTestUtil dictForARHitResult:result]];
                    }
                    resolve(returnArray);
                }
            }
        }
    }];
}


@end
