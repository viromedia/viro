//
//  VRTController.h
//  React
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <React/RCTBridge.h>
#import "VRTView.h"


@interface VRTController : VRTView<VROEventDelegateProtocol>

@property (nonatomic, copy, nullable) RCTDirectEventBlock onClickViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onFuseViro;
@property (nonatomic, assign) BOOL canClick;
@property (nonatomic, assign) BOOL canFuse;

- (instancetype) initWithBridge:(RCTBridge *)bridge;
- (void) onClick:(int)source clickState:(VROEventDelegate::ClickState)clickState;

@end