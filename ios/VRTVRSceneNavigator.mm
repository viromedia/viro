//
//  VRTVRSceneNavigator.m
//  ViroReact
//
//  Created by Manish Bodhankar on 3/30/18.
//  Copyright © 2018 Viro Media. All rights reserved.
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

#import <ViroKit/ViroKit.h>
#import <React/RCTAssert.h>
#import <React/RCTLog.h>
#import "VRTVRSceneNavigator.h"
#import "VRTScene.h"
#import "VRTNotifications.h"
#import <React/RCTRootView.h>
#import <React/RCTUtils.h>
#import "VRTPerfMonitor.h"
#import "VRTMaterialManager.h"
#import "VRTMaterialVideo.h"

/**
 VRTSceneNavigator manages the various scenes that a Viro App can navigate between.
 **/

@implementation VRTVRSceneNavigator {
  id <VROView> _vroView;
  VROViewControllerGVR *_gvrController;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _hdrEnabled = YES;
        _pbrEnabled = YES;
        _bloomEnabled = YES;
        _shadowsEnabled = YES;
        _multisamplingEnabled = NO;
    }
    return self;
}

/*
 We defer creating the VR view until either the Scene is added or
 VR mode is set. This way we're able to disable VR mode prior to creating
 the view, which is important because if VR mode is enabled we force
 landscape mode, and if VR mode is not we do not.
 
 Ideally we'd be able to set VR mode after the fact, but GVR bugs prevent
 this (the double-viewport issue). Return YES if we created a new _vroView, NO if
 one already exists and the method was a no-op.
 */
- (BOOL)initVRView {
    if (_vroView != nil) {
        return NO;
    }
    VRORendererConfiguration config;
    config.enableHDR = _hdrEnabled;
    config.enablePBR = _pbrEnabled;
    config.enableBloom = _bloomEnabled;
    config.enableShadows = _shadowsEnabled;
    config.enableMultisampling = _multisamplingEnabled;
    
    _gvrController = [[VROViewControllerGVR alloc] initWithConfig:config];
    _gvrController.forceLandscape = _vrModeEnabled;
    _vroView = (id<VROView>) _gvrController.view;
    
    // Load materials; must be done each time we have a new context (e.g. after
    // the EGL context is created by the VROViewGVR
    VRTMaterialManager *materialManager = [self.bridge materialManager];
    [materialManager reloadMaterials];
    VROViewGVR *viewCardboard = (VROViewGVR *) _gvrController.view;
    [viewCardboard setAutoresizingMask:UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight];
    _vroView.renderDelegate = self;
    
    [self setFrame:[UIScreen mainScreen].bounds];
    [self setAutoresizingMask:UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight];
    
    [self addSubview:viewCardboard];
    self.currentViews = [[NSMutableArray alloc] init];
    [self.bridge.perfMonitor setView:_vroView];
    
    return YES;
}

- (void)setVrModeEnabled:(BOOL)enabled {
  _vrModeEnabled = enabled;
  [self initVRView];
  [_vroView setVrMode:_vrModeEnabled];
}

- (void)setHdrEnabled:(BOOL)hdrEnabled {
    _hdrEnabled = hdrEnabled;
    if (_vroView) {
        [_vroView setHDREnabled:hdrEnabled];
    }
}

- (void)setPbrEnabled:(BOOL)pbrEnabled {
    _pbrEnabled = pbrEnabled;
    if (_vroView) {
        [_vroView setPBREnabled:pbrEnabled];
    }
}

- (void)setBloomEnabled:(BOOL)bloomEnabled {
    _bloomEnabled = bloomEnabled;
    if (_vroView) {
        [_vroView setBloomEnabled:bloomEnabled];
    }
}

- (void)setShadowsEnabled:(BOOL)shadowsEnabled {
    _shadowsEnabled = shadowsEnabled;
    if (_vroView) {
        [_vroView setShadowsEnabled:shadowsEnabled];
    }
}

- (void)setMultisamplingEnabled:(BOOL)multisamplingEnabled {
    _multisamplingEnabled = multisamplingEnabled;
}

- (void)recenterTracking {
  [self initVRView];
  VROViewGVR *cardboardView = _vroView;
  [cardboardView recenterTracking];
}

- (UIView *)rootVROView {
  [self initVRView];
  return (UIView *)_vroView;
}

//VROComponent overrides...
- (void)insertReactSubview:(UIView *)subview atIndex:(NSInteger)atIndex {
  RCTAssert([subview isKindOfClass:[VRTScene class]], @"VRTSceneNavigator only accepts VRTScene subviews");
  VRTScene *sceneView = (VRTScene *)subview;
  BOOL isVRViewInit = [self initVRView];
  //if initVRView ran then re apply materials.
  if (isVRViewInit == YES) {
    [self applyMaterialsToSceneChildren:sceneView];
  }
  
  [sceneView setView:_vroView];
  [self.currentViews insertObject:sceneView atIndex:atIndex];
  
  if (self.currentSceneIndex == atIndex) {
    [self setSceneView:sceneView];
    [(VROViewGVR *)_vroView setPaused:NO];
  }
  [super insertReactSubview:subview atIndex:atIndex];
}

-(void)setCurrentSceneIndex:(NSInteger)index {
  int currentViewsLength = (int)[_currentViews count];
  _currentSceneIndex = index;
  
  if (_currentSceneIndex < 0 || _currentSceneIndex > (currentViewsLength - 1)){
    // setCurrentSceneTag may be set before insertReactSubView class.
    // In this case, just return.
    return;
  }
  
  VRTScene *sceneView = [_currentViews objectAtIndex:index];
  [self setSceneView:sceneView];
}

- (void)removeReactSubview:(UIView *)subview {
  VRTScene *sceneView = (VRTScene *)subview;
  [self.currentViews removeObject:sceneView];
  [super removeReactSubview:subview];
}

- (NSArray *)reactSubviews {
  return self.currentViews;
}

- (UIView *)reactSuperview{
  return nil;
}

#pragma mark - VRORenderDelegate methods

- (void)setupRendererWithDriver:(std::shared_ptr<VRODriver>)driver {
  
}

- (void)userDidRequestExitVR {
  // Notify javascript listeners (for ReactNativeJs to ViroReactJs cases)
  if (self.onExitViro != nil) {
    self.onExitViro(nil);
  }
  
  // Notify Native listeners (for NativeApp to ViroReactJs cases)
  [[NSNotificationCenter defaultCenter] postNotification:[NSNotification notificationWithName:kVRTOnExitViro object:nil]];
}

- (void)setSceneView:(VRTScene *)sceneView {
  if (_currentScene == sceneView) {
    return;
  }
  
  if (_currentScene == nil) {
    [_vroView setSceneController:[sceneView sceneController]];
  } else {
    [_vroView setSceneController:[sceneView sceneController] duration:1 timingFunction:VROTimingFunctionType::EaseIn];
  }
  _currentScene = sceneView;
}

// recursively traverse down the scene tree to apply materials. This is invoked after reloaded materials from initVRView() to ensure components are applying the latest reloaded materials.
- (void)applyMaterialsToSceneChildren:(VRTView *)view {
  NSArray *children = [view reactSubviews];
  if (children != nil && children.count > 0) {
    for (VRTView *childView in children) {
      if ([childView respondsToSelector:@selector(applyMaterials)]) {
        if ([childView isKindOfClass:[VRTNode class]]) {
          VRTNode *node = (VRTNode *)childView;
          [node applyMaterials];
        }
      }
      [self applyMaterialsToSceneChildren:childView];
    }
  }
}

- (void)removeFromSuperview {
  [self parentDidDisappear];
  [super removeFromSuperview];
  
  /*
   We need to always ensure that React's root view is showing when we
   are deallocating Viro and our renderer. This is because GVR does not
   perform the proper cleanup on its windows: VIRO-1067
   */
  if (RCTIsMainQueue()){
    NSArray *windowArray = [UIApplication sharedApplication].windows;
    if (windowArray == nil || [windowArray count] == 0) {
      return;
    }
    
    /*
     Search the top level rootView controllers and their children to see if their views are
     RCTRootView. If so, then make that root view visible. This is a hack that
     removes GVR's frozen frame referenced in VIRO-1067 during shutdown.
     */
    for (int i = 0; i < [windowArray count]; i ++) {
      UIWindow *window = [windowArray objectAtIndex:i];
      if (window != nil && window.rootViewController != nil) {
        if (window.rootViewController.view != nil && [self findRCTRootView:window.rootViewController] != nil) {
          [window makeKeyAndVisible];
          return;
        }
      }
    }
    
    /*
     If we are still here we didn't make any window visible over GVR. Make first one with a valid
     rootViewController visible.
     */
    for (int i = 0; i < [windowArray count]; i++) {
      UIWindow *window = [windowArray objectAtIndex:i];
      if (window != nil && window.rootViewController != nil) {
        [window makeKeyAndVisible];
        return;
      }
    }
  }
}

// Return the RCTRootView, if any, descending from the given controller
-(RCTRootView *) findRCTRootView:(UIViewController *)controller {
  if ([controller.view isKindOfClass:[RCTRootView class]]) {
    return (RCTRootView *)controller.view;
  }
  for (UIView *view in controller.view.subviews) {
    if([view isKindOfClass:[RCTRootView class]]) {
      return (RCTRootView *)view;
    }
  }
  if (controller.childViewControllers == nil) {
    return nil;
  }
  
  for (UIViewController *controllerChild in controller.childViewControllers) {
    if (controllerChild.view != nil) {
      if ([controllerChild.view isKindOfClass:[RCTRootView class]]) {
        return (RCTRootView *)controllerChild.view;
      }
      for (UIView *view in controllerChild.view.subviews) {
        if ([view isKindOfClass:[RCTRootView class]]) {
          return (RCTRootView *)view;
        }
      }
    }
    
    RCTRootView *potentialRootView = [self findRCTRootView:controllerChild];
    if (potentialRootView != nil) {
      return potentialRootView;
    }
  }
  return nil;
}

#pragma mark RCTInvalidating methods

- (void)invalidate {
  //NOTE: DO NOT NULL OUT _currentViews here, that will cause a memory leak and prevent child views from
  //being released.
  //_currentViews = nil;
  _currentScene = nil;
  _vroView = nil;
  _childViews = nil;
}
@end
