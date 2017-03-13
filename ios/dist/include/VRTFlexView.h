//
//  VRTFlexView.h
//  React
//
//  Created by Vik Advani on 5/9/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <React/RCTBridge.h>
#import "VRTNode.h"


@interface VRTFlexView : VRTNode

@property (nonatomic, assign) float width;
@property (nonatomic, assign) float height;
@property (nonatomic, strong) UIColor *backgroundColor; // Used to propagate to children

- (instancetype)initWithBridge:(RCTBridge *)bridge;

@end
