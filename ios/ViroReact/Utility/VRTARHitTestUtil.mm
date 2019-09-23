//
//  VRTARHitTestUtil.m
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
