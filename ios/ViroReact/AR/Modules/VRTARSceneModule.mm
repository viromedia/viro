//
//  VRTARSceneModule.mm
//  ViroReact
//
//  Created by Andy Chu on 8/9/17.
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
#import <React/RCTUIManagerUtils.h>
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
                    VROVector3f rayVector = VROVector3f([[ray objectAtIndex:0] floatValue],
                                                        [[ray objectAtIndex:1] floatValue],
                                                        [[ray objectAtIndex:2] floatValue]);
                    std::vector<std::shared_ptr<VROARHitTestResult>> results = [view performARHitTest:rayVector];
                    
                    NSMutableArray *returnArray = [[NSMutableArray alloc] initWithCapacity:results.size()];
                    for (std::shared_ptr<VROARHitTestResult> &result : results) {
                        [returnArray addObject:[VRTARHitTestUtil dictForARHitResult:result]];
                    }
                    resolve(returnArray);
                }
            }
        }
    }];
}

RCT_EXPORT_METHOD(performARHitTestWithWorldPoints:(nonnull NSNumber *)viewTag
                  ray:(NSArray *)ray
                  resolver:(RCTPromiseResolveBlock)resolve
                  rejecter:(RCTPromiseRejectBlock)reject) {
    // No-op for iOS, used only in Android
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
                    std::vector<std::shared_ptr<VROARHitTestResult>> results = [view performARHitTest:(targetPosition - cameraPosition)];
                    NSMutableArray *returnArray = [[NSMutableArray alloc] initWithCapacity:results.size()];
                    for (std::shared_ptr<VROARHitTestResult> &result : results) {
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
                    std::vector<std::shared_ptr<VROARHitTestResult>> results = [view performARHitTestWithPoint:x y:y];
                    
                    NSMutableArray *returnArray = [[NSMutableArray alloc] initWithCapacity:results.size()];
                    for (std::shared_ptr<VROARHitTestResult> &result : results) {
                        [returnArray addObject:[VRTARHitTestUtil dictForARHitResult:result]];
                    }
                    resolve(returnArray);
                }
            }
        }
    }];
}


@end
