//
//  VRTPerfMonitor.m
//  ViroReact
//
//  Created by Raj Advani on 3/28/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import "VRTPerfMonitor.h"
#import <React/RCTDevMenu.h>
#import <React/RCTUIManager.h>
#import <React/RCTUIManagerUtils.h>

@implementation VRTPerfMonitor {
    
    BOOL _isShowing;
    RCTDevMenuItem *_devMenuItem;
    
}

@synthesize bridge = _bridge;

RCT_EXPORT_MODULE()

- (instancetype)init
{
    _isShowing = NO;
    
    // Override to ensure the module gets created at startup
    // Remove once module setup is more declarative (Facebook t11106126).
    return [super init];
}

- (void)setBridge:(RCTBridge *)bridge
{
    _bridge = bridge;
    [bridge.devMenu addItem:self.devMenuItem];
}

- (BOOL)isShowing {
    return _isShowing;
}

- (void)show {
    _isShowing = YES;
    [_view setDebugHUDEnabled:YES];
}

- (void)hide {
    _isShowing = NO;
    [_view setDebugHUDEnabled:NO];
}

- (void)setView:(VROViewGVR *)view {
    _view = view;
    if (_isShowing) {
        [self show];
    }
    else {
        [self hide];
    }
}

- (dispatch_queue_t)methodQueue {
    return RCTGetUIManagerQueue();
}

- (RCTDevMenuItem *)devMenuItem
{
    if (!_devMenuItem) {
        __weak __typeof__(self) weakSelf = self;
        _devMenuItem =
        [RCTDevMenuItem buttonItemWithTitleBlock:^NSString *{
            return [weakSelf isShowing] ? @"[Viro] Hide FPS" : @"[Viro] Display FPS";
        } handler:^{
            if ([weakSelf isShowing]) {
                [weakSelf hide];
            } else {
                [weakSelf show];
            }
        }];
    }
    
    return _devMenuItem;
}

@end

@implementation RCTBridge (VRTPerfMonitor)

- (VRTPerfMonitor *)perfMonitor
{
    return [self moduleForClass:[VRTPerfMonitor class]];
}

@end
