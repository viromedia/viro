//
//  VRTPerfMonitor.h
//  ViroReact
//
//  Created by Raj Advani on 3/28/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <ViroKit/ViroKit.h>
#import <React/RCTBridgeModule.h>
#import <React/RCTBridge.h>

@interface VRTPerfMonitor : NSObject <RCTBridgeModule>

@property (readwrite, nonatomic, weak) VROViewCardboard *view;

@end

@interface RCTBridge (VRTPerfMonitor)

- (VRTPerfMonitor *)perfMonitor;

@end
