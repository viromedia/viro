//
//  VRTARPlane.h
//  ViroReact
//
//  Created by Andy Chu on 6/15/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTARAnchorNode.h"

@interface VRTARPlane : VRTARAnchorNode

@property (nonatomic, assign) float minWidth;
@property (nonatomic, assign) float minHeight;
@property (nonatomic, copy, nullable) NSString *alignment;

@property (readwrite, nonatomic) std::shared_ptr<VROARNodeDelegateiOS> arNodeDelegate;

- (instancetype)initWithBridge:(RCTBridge *)bridge;

@end
