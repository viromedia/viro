//
//  VRTARUtils.h
//  ViroReact
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ViroKit/ViroKit.h>
#import <React/RCTBridgeModule.h>

@interface VRTARUtils : NSObject<RCTBridgeModule>

+ (NSDictionary *)createDictionaryFromAnchor:(std::shared_ptr<VROARAnchor>) anchor;

+ (NSDictionary *)createDictionaryFromARPointCloud:(std::shared_ptr<VROARPointCloud>) pointCloud;

@end
