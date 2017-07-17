//
//  VRTControl.m
//  React
//
//  Created by Vik Advani on 9/15/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <ViroKit/ViroKit.h>
#import "VRTControl.h"
#import <React/RCTLog.h>

@implementation VRTControl {
    
}

- (void)insertReactSubview:(UIView *)view atIndex:(NSInteger)atIndex {
    RCTLogError(@"A control component cannot have children. Only containers such <ViroNode> or <ViroFlexView> are allowed to have children");
}

@end;
