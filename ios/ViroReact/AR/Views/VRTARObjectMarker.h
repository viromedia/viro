//
//  VRTARObjectMarker.h
//  ViroReact
//
//  Created by Andy Chu on 8/10/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTARAnchorNode.h"

@interface VRTARObjectMarker : VRTARAnchorNode

@property (nonatomic, copy, nullable) NSString *target;

@property (readwrite, nonatomic) std::shared_ptr<VROARNodeDelegateiOS> arNodeDelegate;

- (instancetype)initWithBridge:(RCTBridge *)bridge;

@end
