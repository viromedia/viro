//
//  VRTCamera.h
//  React
//
//  Created by Raj Advani on 10/5/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTControl.h"
#import "RCTBridge.h"

@interface VRTCamera : VRTControl

- (instancetype)initWithBridge:(RCTBridge *)bridge;

- (VROCameraRotationType)rotationType;

- (void)setPosition:(NSArray<NSNumber *> *)position;

@property (nonatomic, copy) NSArray<NSNumber *> *position;
@property (readonly, nonatomic) std::shared_ptr<VRONodeCamera> nodeCamera;

@end
