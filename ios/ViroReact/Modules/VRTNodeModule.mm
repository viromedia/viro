//
//  VRTNodeModule
//  ViroReact
//
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
#import "VRTNodeModule.h"
#import "VRTNode.h"
#import "VRT3DObject.h"
#import <React/RCTUIManagerUtils.h>
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
        VROBoundingBox boundingBox = vroNode->getUmbrellaBoundingBox();
        
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

RCT_EXPORT_METHOD(getMorphTargets:(nonnull NSNumber *)viewTag
                  resolve:(RCTPromiseResolveBlock)resolve
                  reject:(RCTPromiseRejectBlock)reject) {
    [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
        UIView *nodeView = viewRegistry[viewTag];
        
        if (![nodeView isKindOfClass:[VRT3DObject class]]) {
            RCTLogError(@"Invalid view, expected VRT3DObject, got [%@]", nodeView);
            return;
        }
        
        VRTNode *node = (VRT3DObject *) nodeView;
        std::shared_ptr<VRONode> vroNode = [node node];
        std::set<std::shared_ptr<VROMorpher>> morphers = vroNode->getMorphers(true);
        std::set<std::string> keys;
        for (auto morph : morphers) {
            for (auto key : morph->getMorphTargetKeys()) {
                keys.insert(key);
            }
        }
        
        NSMutableArray *returnArray = [[NSMutableArray alloc] initWithCapacity:keys.size()];
        for (auto key : keys) {
            NSString* result = [NSString stringWithUTF8String:key.c_str()];
            [returnArray addObject:result];
        }
        
        resolve(@{@"targets" : returnArray});
    }];
}

@end
