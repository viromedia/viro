//
//  VRTEventDelegate.h
//  React
//
//  Created by Vik Advani on 3/1/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>


@class VRTView;

@protocol VRTEventDelegate<NSObject>

-(void)onTap:(VRTView *)view;
-(void)onGaze:(VRTView *)view state:(BOOL)isGazing;

@end
