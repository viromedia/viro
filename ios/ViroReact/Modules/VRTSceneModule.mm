//
//  VRTSceneModule.m
//  ViroReact
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <React/RCTUIManager.h>
#import "VRTScene.h"
#import "VRTSceneModule.h"

@implementation VRTSceneModule
@synthesize bridge = _bridge;

RCT_EXPORT_MODULE()

- (dispatch_queue_t)methodQueue {
    return RCTGetUIManagerQueue();
}

RCT_EXPORT_METHOD(findCollisionsWithRayAsync:(nonnull NSNumber *)viewTag
                  from:(NSArray *) fromArray
                  to:(NSArray *) toArray
                  returnClosests:(BOOL) returnClosests
                  rayTag:(NSString *)tag
                  resolver:(RCTPromiseResolveBlock)resolve
                  rejecter:(RCTPromiseRejectBlock)reject) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        UIView *sceneView = viewRegistry[viewTag];
        if (![sceneView isKindOfClass:[VRTScene class]]) {
            RCTLogError(@"Invalid view returned when calling findCollisionsWithRayAsync: expected VRTScene, got [%@]", sceneView);
        } else if ([fromArray count] != 3 || [toArray count] != 3) {
            RCTLogError(@"Invalid From / To positions or tag provided for findCollisionsWithRayAsync!");
        } else {
            std::string strTag = "";
            if (tag != nil) {
                strTag = std::string([tag UTF8String]);
            }
            VRTScene *scene = (VRTScene *)sceneView;
            VROVector3f from = [self NSArrayToVector3fHelper:fromArray];
            VROVector3f to = [self NSArrayToVector3fHelper:toArray];
            bool hasHit = [scene scene]->getPhysicsWorld()->findCollisionsWithRay(from, to, returnClosests, strTag);
            resolve(@(hasHit));
        }
    }];
}

RCT_EXPORT_METHOD(findCollisionsWithShapeAsync:(nonnull NSNumber *)viewTag
                  from:(NSArray *) fromArray
                  to:(NSArray *) toArray
                  shape:(NSString *) shapeTypeString
                  params:(NSArray *) shapeParams
                  rayTag:(NSString *)tag
                  resolver:(RCTPromiseResolveBlock)resolve
                  rejecter:(RCTPromiseRejectBlock)reject) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        UIView *sceneView = viewRegistry[viewTag];
        std::shared_ptr<VROPhysicsShape> shape = [VRTNode getPhysicsShape:shapeTypeString params:shapeParams];
        if (![sceneView isKindOfClass:[VRTScene class]]) {
            RCTLogError(@"Invalid view returned when calling findCollisionsWithShapeAsync: expected VRTScene, got [%@]", sceneView);
        } else if ([fromArray count] != 3 || [toArray count] != 3 || tag == nil) {
            RCTLogError(@"Invalid From / To positions provided for findCollisionsWithShapeAsync!");
        } else if (shape != nullptr) {
            std::string strTag = "";
            if (tag != nil){
                strTag = std::string([tag UTF8String]);
            }

            VRTScene *scene = (VRTScene *)sceneView;
            VROVector3f from = [self NSArrayToVector3fHelper:fromArray];
            VROVector3f to = [self NSArrayToVector3fHelper:toArray];
            bool hasHit = [scene scene]->getPhysicsWorld()->findCollisionsWithShape(from, to, shape, strTag);
            resolve(@(hasHit));
        }
    }];
}

- (VROVector3f)NSArrayToVector3fHelper:(NSArray *)array{
    float x = [[array objectAtIndex:0] floatValue];
    float y = [[array objectAtIndex:1] floatValue];
    float z = [[array objectAtIndex:2] floatValue];
    VROVector3f from = VROVector3f(x, y, z);
    return from;
}
@end
