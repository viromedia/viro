//
//  VRT3DSceneNavigator.m
//  ViroReact
//
//  Created by Manish Bodhankar on 3/12/18.
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
#import "VRT3DSceneNavigator.h"
#import "VRTScene.h"
#import "VRTNotifications.h"
#import <React/RCTRootView.h>
#import <React/RCTUtils.h>
#import "VRTPerfMonitor.h"
#import "VRTMaterialManager.h"

/**
 VRT3DSceneNavigator manages the various scenes that a Viro App can navigate between.
 **/

@implementation VRT3DSceneNavigator {
  id <VROView> _vroView;
  RCTBridge *_bridge;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        VRTMaterialManager *materialManager = [bridge materialManager];
        [materialManager reloadMaterials];

        [self setFrame:CGRectMake(0, 0,
                                  [[UIScreen mainScreen] bounds].size.width,
                                  [[UIScreen mainScreen] bounds].size.height)];
        self.currentViews = [[NSMutableArray alloc] init];
        
        _hdrEnabled = YES;
        _pbrEnabled = YES;
        _bloomEnabled = YES;
        _shadowsEnabled = YES;
        _multisamplingEnabled = NO;
    }
    return self;
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
  // if we haven't created the VROView, then create it now that
  // all the props have been set.
  if (!_vroView) {
      EAGLContext *context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    
      VRORendererConfiguration config;
      config.enableHDR = _hdrEnabled;
      config.enablePBR = _pbrEnabled;
      config.enableBloom = _bloomEnabled;
      config.enableShadows = _shadowsEnabled;
      config.enableMultisampling = _multisamplingEnabled;
    
      _vroView = [[VROViewScene alloc] initWithFrame:CGRectMake(0, 0,
                                                                [[UIScreen mainScreen] bounds].size.width,
                                                                [[UIScreen mainScreen] bounds].size.height)
                                              config:config context:context] ;
      VROViewScene *viewScene = (VROViewScene *) _vroView;
      [viewScene setAutoresizingMask:UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight];
      _vroView.renderDelegate = self;
      
      [self addSubview:(UIView *)_vroView];
      
      [_bridge.perfMonitor setView:_vroView];

      
      // set the scene if it was set before this view was created
      if (_currentScene) {
          [_currentScene setView:_vroView];
          [_vroView setSceneController:[_currentScene sceneController]];
      }
  }
}

- (void)recenterTracking {

  VROViewScene *view = _vroView;
  [view recenterTracking];
}

- (UIView *)rootVROView {

  return (UIView *)_vroView;
}

//VROComponent overrides...
- (void)insertReactSubview:(UIView *)subview atIndex:(NSInteger)atIndex {

  RCTAssert([subview isKindOfClass:[VRTScene class]], @"VRTSceneNavigator only accepts VRTScene subviews");
  [super insertReactSubview:subview atIndex:atIndex];
  VRTScene *sceneView = (VRTScene *)subview;
  
  [sceneView setView:_vroView];
  [self.currentViews insertObject:sceneView atIndex:atIndex];
  
  if (self.currentSceneIndex == atIndex) {
    [self setSceneView:sceneView];
  }
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

- (void)removeFromSuperview {
  [self parentDidDisappear];
  if (_vroView) {
    VROViewScene *viewScene = (VROViewScene *)_vroView;
    [viewScene deleteGL];
  }
  [super removeFromSuperview];
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

/*
 Unproject the given screen coordinates into world coordinates. The given screen coordinate vector must
 contain a Z element in the range [0,1], where 0 is the near clipping plane and 1 the far clipping plane.
 */
-(VROVector3f) unprojectPoint:(VROVector3f)point {
    if(_vroView == nil || _vroView.renderer == nil) {
        RCTLogError(@"Unable to unproject. Renderer not intialized");
    }

    VROVector3f unprojectedPoint = [_vroView unprojectPoint:point];
    return unprojectedPoint;
}

// Project the given world coordinates into screen coordinates.
-(VROVector3f) projectPoint:(VROVector3f)point {
    if(_vroView == nil || _vroView.renderer == nil) {
        RCTLogError(@"Unable to unproject. Renderer not intialized");
    }

    VROVector3f projectedPoint = [_vroView projectPoint:point];
    return projectedPoint;
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
