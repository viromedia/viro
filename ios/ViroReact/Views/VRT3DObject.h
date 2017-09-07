//
//  VRT3DObject.h
//  React
//
//  Created by Vik Advani on 4/14/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <React/RCTBridge.h>
#import "VRTControl.h"


@interface VRT3DObject : VRTControl

- (instancetype)initWithBridge:(RCTBridge *)bridge;

@property (readwrite, nonatomic) NSDictionary *source;
@property (readwrite, nonatomic) NSDictionary *animation;
@property (readwrite, nonatomic) NSString *type;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onLoadStartViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onLoadEndViro;
@property (nonatomic, copy) RCTDirectEventBlock onErrorViro;
@property (nonatomic, copy) RCTDirectEventBlock onAnimationStartViro;
@property (nonatomic, copy) RCTDirectEventBlock onAnimationFinishViro;

@end
