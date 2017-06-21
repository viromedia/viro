//
//  VRTARPlane.h
//  ViroReact
//
//  Created by Andy Chu on 6/15/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTARNode.h"

@interface VRTARPlane : VRTARNode

// default min height/width is 0, we match any surface
@property (nonatomic, assign) float minWidth;
@property (nonatomic, assign) float minHeight;

- (instancetype)initWithBridge:(RCTBridge *)bridge;

@end
