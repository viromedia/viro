//
//  VRTARUtils.m
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

#import "VRTARUtils.h"

@implementation VRTARUtils

RCT_EXPORT_MODULE();

RCT_EXPORT_METHOD(isARSupported:(RCTResponseSenderBlock)callback)
{
  bool result = [VROViewAR isARSupported];
  NSDictionary *props = @{@"isARSupported" : @(result)};
  callback(@[[NSNull null], props]);
}

+ (NSDictionary *)createDictionaryFromAnchor:(std::shared_ptr<VROARAnchor>) anchor {
    
    NSMutableDictionary *dict = [[NSMutableDictionary alloc] init];
    
    [dict setObject:[NSString stringWithUTF8String:anchor->getId().c_str()] forKey:@"anchorId"];
    
    VROMatrix4f transform =  anchor->getTransform();
    VROVector3f position = transform.extractTranslation();
    VROVector3f scale = transform.extractScale();
    VROVector3f rotation = transform.extractRotation(scale).toEuler();
    
    [dict setObject:@[@(position.x), @(position.y), @(position.z)] forKey:@"position"];
    [dict setObject:@[@(scale.x), @(scale.y), @(scale.z)] forKey:@"scale"];
    [dict setObject:@[@(toDegrees(rotation.x)), @(toDegrees(rotation.y)), @(toDegrees(rotation.z))] forKey:@"rotation"];

    // default type is "anchor", override below.
    [dict setObject:@"anchor" forKey:@"type"];
    
    std::shared_ptr<VROARPlaneAnchor> planeAnchor = std::dynamic_pointer_cast<VROARPlaneAnchor>(anchor);
    if (planeAnchor) {
        [dict setObject:@"plane" forKey:@"type"];
        [dict setObject:@[@(planeAnchor->getCenter().x), @(planeAnchor->getCenter().y), @(planeAnchor->getCenter().z)] forKey:@"center"];
        [dict setObject:@(planeAnchor->getExtent().x) forKey:@"width"];
        [dict setObject:@(planeAnchor->getExtent().z) forKey:@"height"];
        
        // Set polygon vertices points
        std::vector<VROVector3f> points = planeAnchor->getBoundaryVertices();
        NSMutableArray *pointsArray = [[NSMutableArray alloc] initWithCapacity:points.size()];
        for (VROVector3f point : points) {
            [pointsArray addObject:@[@(point.x), @(point.y), @(point.z)]];
        }
        [dict setObject:pointsArray forKey:@"vertices"];

        switch (planeAnchor->getAlignment()) {
            case VROARPlaneAlignment::Vertical:
                [dict setObject:@"Vertical" forKey:@"alignment"];
                break;
            case VROARPlaneAlignment::Horizontal:
            default:
                [dict setObject:@"Horizontal" forKey:@"alignment"];
                break;
        }
    }
    
    return dict;
}

+ (NSDictionary *)createDictionaryFromARPointCloud:(std::shared_ptr<VROARPointCloud>) pointCloud {
    NSMutableDictionary *dict = [[NSMutableDictionary alloc] init];

    std::vector<VROVector4f> points = pointCloud->getPoints();
    NSMutableArray *pointsArray = [[NSMutableArray alloc] initWithCapacity:points.size()];
    
    // note: the 4th value of the VROVector4f is a "confidence" value only meaningful in Android.
    for (VROVector4f point : points) {
        [pointsArray addObject:@[@(point.x), @(point.y), @(point.z), @(point.w)]];
    }

    std::vector<uint64_t> identifiers = pointCloud->getIdentifiers();
    NSMutableArray *identifiersArray = [[NSMutableArray alloc] initWithCapacity:identifiers.size()];

    for (uint64_t identifier : identifiers) {
        [identifiersArray addObject:[NSNumber numberWithUnsignedLongLong:identifier]];
    }

    [dict setObject:pointsArray forKey:@"points"];
    [dict setObject:identifiersArray forKey:@"identifiers"];

    return dict;
}

@end
