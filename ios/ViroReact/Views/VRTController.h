//
//  VRTController.h
//  React
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTView.h"
#import "RCTBridge.h"

@interface VRTController : VRTView<VROEventDelegateProtocol>

@property (nonatomic, copy, nullable) RCTDirectEventBlock onClickViro;
@property (nonatomic, assign) BOOL canClick;

- (instancetype) initWithBridge:(RCTBridge *)bridge;
- (void) onClick:(int)source clickState:(VROEventDelegate::ClickState)clickState;

@end