//
//  VRTPolygon.h
//  ViroReact
//
//  Copyright © 2018 Viro Media. All rights reserved.
//

#import "VRTControl.h"

@interface VRTPolygon : VRTControl
@property (nonatomic, copy, nullable) NSArray<NSArray<NSNumber *> *> *vertices;
@property (nonatomic, copy, nullable) NSArray<NSArray<NSArray<NSNumber *> *> *> *holes;
@property (nonatomic, copy, nullable) NSArray<NSNumber *> *uvCoordinates;
@property (nonatomic, assign) BOOL arShadowReceiver;

-(instancetype)initWithBridge:(RCTBridge *)bridge;

@end
