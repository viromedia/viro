//
//  VRTNodeModule
//  ViroReact
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <React/RCTUIManager.h>
#import "VRTNodeModule.h"
#import "VRTNode.h"

@implementation VRTNodeModule

@synthesize bridge = _bridge;

RCT_EXPORT_MODULE()

- (dispatch_queue_t)methodQueue {
    return RCTGetUIManagerQueue();
}

RCT_EXPORT_METHOD(applyImpulse:(nonnull NSNumber *)viewTag
                  withForce:(NSArray<NSNumber *> *)forceImpulse
                  atPosition:(NSArray<NSNumber *> *)position) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        UIView *nodeView = viewRegistry[viewTag];
        if (![nodeView isKindOfClass:[VRTNode class]]) {
            RCTLogError(@"Invalid view returned when applying force: expected a node-type control, got [%@]", nodeView);
            return;
        }
        
        // Veify that the right impulse force is given
        if (forceImpulse == nil || [forceImpulse count] != 3) {
            RCTLogError(@"Invalid force impulse paramters provided!");
            return;
        }
        
        // If no offset is given, default to a central impulse.
        VROVector3f position3f;
        if (position == nil) {
            position3f = VROVector3f(0,0,0);
        }
        
        if (position != nil && [position count] != 3) {
            RCTLogError(@"Invalid force impulse position paramters provided!");
            return;
        } else {
            position3f = VROVector3f([[position objectAtIndex:0] floatValue],
                                     [[position objectAtIndex:1] floatValue],
                                     [[position objectAtIndex:2] floatValue]);
            
        }
        
        // If paramters are valid, parse and apply force impulse.
        VROVector3f forceImpulse3f = VROVector3f([[forceImpulse objectAtIndex:0] floatValue],
                                                 [[forceImpulse objectAtIndex:1] floatValue],
                                                 [[forceImpulse objectAtIndex:2] floatValue]);
        VRTNode *node = (VRTNode *)nodeView;
        [node applyImpulse:forceImpulse3f withOffset:position3f];
    }];
}

RCT_EXPORT_METHOD(applyTorqueImpulse:(nonnull NSNumber *)viewTag
                  withTorque:(NSArray<NSNumber *> *)torqueImpulse) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        UIView *nodeView = viewRegistry[viewTag];
        
        if (![nodeView isKindOfClass:[VRTNode class]]) {
            RCTLogError(@"Invalid view returned when applying torque: expected a node-type control, got [%@]", nodeView);
            return;
        }
        
        // Veify that the right impulse force is given
        if (torqueImpulse == nil || [torqueImpulse count] != 3) {
            RCTLogError(@"Invalid torque impulse parameters provided!");
            return;
        }
        
        VROVector3f torqueImpulse3f = VROVector3f([[torqueImpulse objectAtIndex:0] floatValue],
                                                  [[torqueImpulse objectAtIndex:1] floatValue],
                                                  [[torqueImpulse objectAtIndex:2] floatValue]);
        
        // If paramters are valid, parse and apply torque impulse.
        VRTNode *node = (VRTNode *)nodeView;
        [node applyTorqueImpulse:torqueImpulse3f];
    }];
}


RCT_EXPORT_METHOD(setVelocity:(nonnull NSNumber *)viewTag
                  withTorque:(NSArray<NSNumber *> *)velocity) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        UIView *nodeView = viewRegistry[viewTag];
        
        if (![nodeView isKindOfClass:[VRTNode class]]) {
            RCTLogError(@"Invalid view returned when applying velocity: expected a node-type control, got [%@]", nodeView);
            return;
        }
        
        // Veify that the right velocity is given
        if (velocity == nil || [velocity count] != 3) {
            RCTLogError(@"Invalid velocity parameters provided!");
            return;
        }
        
        // If paramters are valid, parse and apply torque impulse.
        VRTNode *node = (VRTNode *)nodeView;
        [node setVelocity:velocity isConstant:NO];
    }];
}

RCT_EXPORT_METHOD(getNodeTransform:(nonnull NSNumber *)viewTag
                  resolve:(RCTPromiseResolveBlock)resolve
                  reject:(RCTPromiseRejectBlock)reject) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        UIView *nodeView = viewRegistry[viewTag];
        
        if (![nodeView isKindOfClass:[VRTNode class]]) {
            RCTLogError(@"Invalid view, expected VRTNode, got [%@]", nodeView);
            return;
        }

        VRTNode *node = (VRTNode *) nodeView;
        std::shared_ptr<VRONode> vroNode = [node node];
        VROMatrix4f transform = vroNode->getLastWorldTransform();
        VROVector3f position = transform.extractTranslation();
        VROVector3f scale = transform.extractScale();
        VROVector3f rotation = transform.extractRotation(scale).toEuler();
        
        resolve(@{
                 @"position" : @[@(position.x), @(position.y), @(position.z)],
                 @"rotation" : @[@(toDegrees(rotation.x)), @(toDegrees(rotation.y)), @(toDegrees(rotation.z))],
                 @"scale" : @[@(scale.x), @(scale.y), @(scale.z)]
                 });
    }];
}

RCT_EXPORT_METHOD(getBoundingBox:(nonnull NSNumber *)viewTag
                  resolve:(RCTPromiseResolveBlock)resolve
                  reject:(RCTPromiseRejectBlock)reject) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        UIView *nodeView = viewRegistry[viewTag];
        
        if (![nodeView isKindOfClass:[VRTNode class]]) {
            RCTLogError(@"Invalid view, expected VRTNode, got [%@]", nodeView);
            return;
        }
        
        VRTNode *node = (VRTNode *) nodeView;
        std::shared_ptr<VRONode> vroNode = [node node];
        VROBoundingBox boundingBox = vroNode->getLastUmbrellaBoundingBox();
        
        resolve(@{
                  @"boundingBox" : @{
                          @"minX" : @(boundingBox.getMinX()),
                          @"maxX" : @(boundingBox.getMaxX()),
                          @"minY" : @(boundingBox.getMinY()),
                          @"maxY" : @(boundingBox.getMaxY()),
                          @"minZ" : @(boundingBox.getMinZ()),
                          @"maxZ" : @(boundingBox.getMaxZ())
                          }
                  });
    }];
}

@end
