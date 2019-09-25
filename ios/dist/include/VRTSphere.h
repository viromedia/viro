//
//  VRTSphere.h
//  React
//
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTControl.h"

@interface VRTSphere : VRTControl

@property (nonatomic, assign) NSUInteger widthSegmentCount;
@property (nonatomic, assign) NSUInteger heightSegmentCount;
@property (nonatomic, assign) float radius;
@property (nonatomic, assign) BOOL facesOutward;

-(instancetype)initWithBridge:(RCTBridge *)bridge;

@end
