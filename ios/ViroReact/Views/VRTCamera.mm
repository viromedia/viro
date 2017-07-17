//
//  VRTCamera.m
//  React
//
//  Created by Raj Advani on 10/5/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTCamera.h"
#import <ViroKit/ViroKit.h>
#import "VRTMaterialManager.h"
#import "VRTUtils.h"

static NSArray *const kDefaultCameraPosition  = @[@0, @0, @0];

@implementation VRTCamera

@synthesize position = _position;

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _nodeCamera = std::make_shared<VRONodeCamera>();
        self.position = kDefaultCameraPosition;
        self.node->setCamera(_nodeCamera);
    }
    
    return self;
}

- (void)setPosition:(NSArray<NSNumber *> *)position {
    _position = [position copy];
    
    float values[3];
    populateFloatArrayFromNSArray(position, values, 3);
    self.nodeCamera->setPosition({values[0], values[1], values[2]});
}

- (VROCameraRotationType)rotationType {
    return VROCameraRotationType::Standard;
}

@end
