//
//  VRT3DSceneNavigator.m
//  ViroReact
//
//  Created by Manish Bodhankar on 3/12/18.
//  Copyright © 2018 Viro Media. All rights reserved.
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

static NSString *const kVRTInvalidAPIKeyMessage = @"The given API Key is either missing or invalid! If you have not signed up for accessing Viro Media platform, please do so at www.viromedia.com. Otherwise, contact info@viromedia.com if you have a valid key and are encountering this error.";

/**
 VRT3DSceneNavigator manages the various scenes that a Viro App can navigate between.
 **/

@implementation VRT3DSceneNavigator {
  id <VROView> _vroView;
  NSString *_apiKey;
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

  }
  return self;
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
  // if we haven't created the VROView, then create it now that
  // all the props have been set.
  if (!_vroView) {
    EAGLContext *context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    VRORendererConfiguration config;

    _vroView = [[VROViewScene alloc] initWithFrame:CGRectMake(0, 0,
                                                              [[UIScreen mainScreen] bounds].size.width,
                                                              [[UIScreen mainScreen] bounds].size.height)
                                            config:config context:context] ;
    VROViewScene *viewScene = (VROViewScene *) _vroView;
    [viewScene setAutoresizingMask:UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight];
    _vroView.renderDelegate = self;

    [self addSubview:(UIView *)_vroView];

    [_bridge.perfMonitor setView:_vroView];

    // reset the API key now that _vroView has been created.
    [self setApiKey:_apiKey];

    // set the scene if it was set before this view was created (not likely)
    if (_currentScene) {
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

- (void)validateAPIKey {
  // Only validate if we have an API key and our view has been created
  if (!_apiKey || !_vroView) {
    return;
  }
  VROViewValidApiKeyBlock completionBlock = ^(BOOL valid) {
    if (!valid) {
      RCTLogError(kVRTInvalidAPIKeyMessage);
      // fire off a notification to let the user know that the API key is invalid.
      [[NSNotificationCenter defaultCenter] postNotification:[NSNotification notificationWithName:kVRTApiKeyIsInvalid object:nil]];
    }
  };
  [_vroView validateApiKey:_apiKey withCompletionBlock:completionBlock];
}

- (void)setApiKey:(NSString *)apiKey {
  _apiKey = apiKey;
  if (_vroView) {
    VROViewValidApiKeyBlock completionBlock = ^(BOOL valid) {
      if (!valid) {
        RCTLogError(kVRTInvalidAPIKeyMessage);
        // fire off a notification to let the user know that the API key is invalid.
        [[NSNotificationCenter defaultCenter] postNotification:[NSNotification notificationWithName:kVRTApiKeyIsInvalid object:nil]];
      }
    };
    [_vroView validateApiKey:apiKey withCompletionBlock:completionBlock];
  }
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
