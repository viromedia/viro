//
//  VRTARHitTestUtil.m
//  ViroReact
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "VRTARHitTestUtil.h"

@implementation VRTARHitTestUtil

static NSString *const kVRTARHitTestTypeKey = @"type";
static NSString *const kVRTARHitTestTransformKey = @"transform";
static NSString *const kVRTARHitTestPositionKey = @"position";
static NSString *const kVRTARHitTestRotationKey = @"rotation";
static NSString *const kVRTARHitTestScaleKey = @"scale";

+ (NSDictionary *)dictForARHitResult:(std::shared_ptr<VROARHitTestResult> &)result {
    NSMutableDictionary *resultDict = [[NSMutableDictionary alloc] init];
    [resultDict setObject:[VRTARHitTestUtil stringForResultType:result->getType()] forKey:kVRTARHitTestTypeKey];
    
    VROMatrix4f worldTransform = result->getWorldTransform();
    VROVector3f scale = worldTransform.extractScale();
    
    NSMutableDictionary *transformDict = [[NSMutableDictionary alloc] init];
    [transformDict setObject:[VRTARHitTestUtil arrayFromVector:worldTransform.extractTranslation()] forKey:kVRTARHitTestPositionKey];
    [transformDict setObject:[VRTARHitTestUtil arrayFromVector:scale] forKey:kVRTARHitTestScaleKey];
    [transformDict setObject:[VRTARHitTestUtil rotationFromVector:worldTransform.extractRotation(scale).toEuler()]
                      forKey:kVRTARHitTestRotationKey];
    [resultDict setObject:transformDict forKey:kVRTARHitTestTransformKey];
    return resultDict;
}

+ (NSString *)stringForResultType:(VROARHitTestResultType)type {
    switch(type) {
        case VROARHitTestResultType::ExistingPlaneUsingExtent:
            return @"ExistingPlaneUsingExtent";
        case VROARHitTestResultType::ExistingPlane:
            return @"ExistingPlane";
        case VROARHitTestResultType::EstimatedHorizontalPlane:
            return @"EstimatedHorizontalPlane";
        case VROARHitTestResultType::FeaturePoint:
            return @"FeaturePoint";
    }
}

+ (NSArray *)arrayFromVector:(VROVector3f)vector {
    return @[@(vector.x), @(vector.y), @(vector.z)];
}

+ (NSArray *)rotationFromVector:(VROVector3f)vector {
    return @[@(toDegrees(vector.x)), @(toDegrees(vector.y)), @(toDegrees(vector.z))];
}

@end
