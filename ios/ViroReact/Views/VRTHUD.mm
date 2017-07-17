//
//  VRTHUD.m
//  React
//
//  Created by Vik Advani on 1/29/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <React/RCTComponent.h>
#import "VRTHUD.h"
#import "VRTHUDComponent.h"

@implementation VRTHUD

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _hudChildren = [[NSMutableArray alloc] initWithCapacity:100];
    }
    
    return self;
}

- (void)insertReactSubview:(UIView *)subview atIndex:(NSInteger)atIndex {
    if([subview conformsToProtocol:@protocol(VRTHUDComponent)]) {
        VRTHUDLabel<VRTHUDComponent> *hudcomp = (VRTHUDLabel<VRTHUDComponent> *)subview;
        [_hudChildren addObject:[hudcomp getUIView]];
    }
    
    //VA: Uncomment later. [super insertReactSubview:subview atIndex:atIndex];
}

- (NSArray *)getChildren {
    NSArray *array = [_hudChildren copy];
    return array;
}

@end
