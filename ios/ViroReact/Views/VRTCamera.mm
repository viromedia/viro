//
//  VRTCamera.m
//  React
//
//  Created by Raj Advani on 10/5/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#import "VRTCamera.h"
#import <ViroKit/ViroKit.h>
#import "VRTMaterialManager.h"
#import "VRTUtils.h"

static NSArray *const kDefaultCameraPosition  = @[@0, @0, @0];

@implementation VRTCamera {
    NSArray<NSNumber *> *_rotation;
}

@synthesize position = _position;

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _nodeCamera = std::make_shared<VRONodeCamera>();
        _nodeRootTransformCamera = [self createVroNode];
        _nodeCamera->setRefNodeToCopyRotation(self.node);
        self.position = kDefaultCameraPosition;
        _nodeRootTransformCamera->setCamera(_nodeCamera);
        _nodeRootTransformCamera->addChildNode(self.node);
        self.nodeAnimation.node = _nodeRootTransformCamera;
    }
    return self;
}

- (void)setPosition:(NSArray<NSNumber *> *)position {
    _position = [position copy];
    float values[3];
    populateFloatArrayFromNSArray(position, values, 3);
    self.nodeRootTransformCamera->setPosition({values[0], values[1], values[2]});
}


- (void)setRotation:(NSArray<NSNumber *> *)rotation {
    _rotation = [rotation copy];
    float rotationValues[3];
    populateFloatArrayFromNSArray(rotation, rotationValues, 3);
    self.nodeRootTransformCamera->setRotation({toRadians(rotationValues[0]), toRadians(rotationValues[1]), toRadians(rotationValues[2])});
}


- (void)setFieldOfView:(float)fov {
    self.nodeCamera->setFieldOfViewY(fov);
}

- (VROCameraRotationType)rotationType {
    return VROCameraRotationType::Standard;
}

@end
