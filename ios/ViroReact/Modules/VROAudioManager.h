//
//  VROAudioManager.h
//  React
//
//  Created by vik.advani on 4/20/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTScene.h"
#import "RCTBridgeModule.h"
#import "RCTBridge.h"

@interface VROAudioManager : NSObject <RCTBridgeModule>

-(void)setCurrentScene:(VRTScene *)scene;

@end

@implementation RCTBridge (VROAudioManager)

- (VROAudioManager *)audioManager
{
  return [self moduleForClass:[VROAudioManager class]];
}

@end

