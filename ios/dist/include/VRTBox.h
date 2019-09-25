//
//  VROBox.h
//  React
//
//  Created by Vik Advani on 12/14/15.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <React/RCTBridge.h>
#import "VRTControl.h"

@interface VRTBox : VRTControl

@property (nonatomic, assign) float width;
@property (nonatomic, assign) float height;
@property (nonatomic, assign) float length;

-(instancetype)initWithBridge:(RCTBridge *)bridge;

@end
