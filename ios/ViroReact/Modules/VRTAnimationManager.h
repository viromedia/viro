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

@interface AnimationProperty : NSObject
@property (nonatomic, copy) NSDictionary *propertyDictionary;
@property (nonatomic, assign) float durationMilliseconds;
@property (nonatomic, assign) float delayMilliseconds;
@property (nonatomic, assign) VROTimingFunctionType functionType;
@end

@interface VRTAnimationManager : NSObject <RCTBridgeModule>

@property (nonatomic, copy) NSDictionary<NSString *, NSDictionary *> *animations;
@property (nonatomic, copy) NSDictionary<NSString *, NSMutableArray *> *animationChains;

- (void)loadAnimations;
- (AnimationProperty *)getAnimationPropertyForName:(NSString *)name;
- (NSArray *)getAnimationChainForName:(NSString *)name;
- (void)mapAnimatedPropertiesToNode:(VRONode *const)node properties:(NSDictionary *)propertyDict propCheckPoints:(NSMutableDictionary *)propCheckPoints;

@end

@implementation RCTBridge (VRTAnimationManager)

- (VRTAnimationManager *)animationManager
{
    return [self moduleForClass:[VRTAnimationManager class]];
}

@end

