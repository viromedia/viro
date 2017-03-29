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

- (void)show {
  _isShowing = YES;
  [_view setDebugHUDEnabled:YES];
}

- (void)hide {
  _isShowing = NO;
  [_view setDebugHUDEnabled:NO];
}

- (void)setView:(VROViewCardboard *)view {
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
    [RCTDevMenuItem toggleItemWithKey:@"ViroFPSToggle"
                                title:@"Display FPS"
                        selectedTitle:@"Hide FPS"
                              handler:
     ^(BOOL selected) {
       if (selected) {
         [weakSelf show];
       } else {
         [weakSelf hide];
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
