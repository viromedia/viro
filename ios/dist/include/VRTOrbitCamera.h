//
//  VRTOrbitCamera.h
//  React
//
//  Created by Raj Advani on 10/5/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTCamera.h"

@interface VRTOrbitCamera : VRTCamera

- (instancetype)initWithBridge:(RCTBridge *)bridge;

- (void)setFocalPoint:(NSArray<NSNumber *> *)focal;
@property (nonatomic, copy) NSArray<NSNumber *> *focalPoint;

@end
