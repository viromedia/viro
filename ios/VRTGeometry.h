//
//  VRTGeometry.h
//  ViroReact
//
//  Created by Raj Advani on 9/10/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <React/RCTBridge.h>
#import "VRTControl.h"

@interface VRTGeometry : VRTControl

@property (nonatomic, copy, nullable) NSArray<NSArray<NSNumber *> *> *vertices;
@property (nonatomic, copy, nullable) NSArray<NSArray<NSNumber *> *> *normals;
@property (nonatomic, copy, nullable) NSArray<NSArray<NSNumber *> *> *texcoords;
@property (nonatomic, copy, nullable) NSArray<NSArray<NSNumber *> *> *triangleIndices;

@end
