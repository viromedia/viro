//
//  VRTSoundModule.h
//  ViroReact
//
//  Created by Andy Chu on 2/2/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ViroKit/ViroKit.h>
#import "RCTBridgeModule.h"

@interface VRTSoundModule : NSObject <RCTBridgeModule>

- (std::shared_ptr<VROSoundData>)dataForName:(NSString *)name;

@end
