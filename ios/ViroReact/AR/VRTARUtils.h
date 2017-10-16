//
//  VRTARUtils.h
//  ViroReact
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ViroKit/ViroKit.h>

@interface VRTARUtils : NSObject

+ (NSDictionary *)createDictionaryFromAnchor:(std::shared_ptr<VROARAnchor>) anchor;

@end
