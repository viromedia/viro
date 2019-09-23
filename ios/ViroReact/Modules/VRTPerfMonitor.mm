//
//  VRTPerfMonitor.m
//  ViroReact
//
//  Created by Raj Advani on 3/28/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

+ (BOOL)requiresMainQueueSetup {
  return NO;
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
