//
//  VRTAnimationManager.h
//  React
//
//  Created by Vik Advani on 2/17/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ViroKit/ViroKit.h>
#import "RCTBridgeModule.h"
#import "RCTBridge.h"
#import "VRTAnimatedComponent.h"


@interface VRTAnimationManager : NSObject <RCTBridgeModule>

@property (nonatomic, copy) NSDictionary<NSString *, NSDictionary *> *animations;

- (void)parseAnimations;
- (std::shared_ptr<VROExecutableAnimation>)animationForName:(NSString *)name;

@end

@implementation RCTBridge (VRTAnimationManager)

- (VRTAnimationManager *)animationManager
{
    return [self moduleForClass:[VRTAnimationManager class]];
}

@end

