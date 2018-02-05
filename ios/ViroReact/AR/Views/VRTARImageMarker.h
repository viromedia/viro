//
//  VRTARImageMarker.h
//  ViroReact
//
//  Created by Andy Chu on 2/2/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTARAnchorNode.h"

@interface VRTARImageMarker : VRTARAnchorNode

@property (nonatomic, copy, nullable) NSString *target;

@property (readwrite, nonatomic) std::shared_ptr<VROARNodeDelegateiOS> arNodeDelegate;

- (instancetype)initWithBridge:(RCTBridge *)bridge;

@end
