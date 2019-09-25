//
//  VRTHUDComponent.h
//  React
//
//  Created by Vik Advani on 1/29/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTNode.h"

@protocol VRTHUDComponent

-(UIView *)getUIView;

@end

@interface VRTHUDLabel : VRTNode<VRTHUDComponent>{

}

@property(nonatomic, retain) UILabel *label;
@property (nonatomic, copy) NSArray<NSNumber *> *position;

- (instancetype)initWithBridge:(RCTBridge *)bridge;

@end