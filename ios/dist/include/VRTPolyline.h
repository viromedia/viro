//
//  VRTPolyline.h
//  ViroReact
//
//  Created by Andy Chu on 5/8/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <React/RCTBridge.h>
#import "VRTControl.h"

@interface VRTPolyline : VRTControl

@property (nonatomic, copy) NSArray<NSArray<NSNumber *> *> *points;
@property (nonatomic, assign) float thickness;
@property (nonatomic, assign) BOOL closed;

-(instancetype)initWithBridge:(RCTBridge *)bridge;

@end
