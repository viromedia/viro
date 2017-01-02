
//
//  VRTSceneNavigator.m
//  React
//
//  Created by Vik Advani on 3/11/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <ViroKit/ViroKit.h>
#import "VRTSceneNavigator.h"
#import "VRTScene.h"
#import "RCTAssert.h"
#import "RCTLog.h"
#import "VROAudioManager.h"
#import "VRTNotifications.h"

static NSString *const kVRTInvalidAPIKeyMessage = @"The given API Key is either missing or invalid! If you have not signed up for the Viro Media alpha program, please do so at www.viromedia.com. Otherwise, contact info@viromedia.com if you have a valid key and are encountering this error.";

/**
 VRTSceneNavigator manages the various scenes that a Viro App can navigate between.
 **/

@implementation VRTSceneNavigator {
  id <VROView> _vroView;
  NSInteger _currentStackPosition;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _vroView = [[VROViewCardboard alloc] initWithFrame:CGRectMake(0, 0,
                                                                      [[UIScreen mainScreen] bounds].size.width,
                                                                      [[UIScreen mainScreen] bounds].size.height)];
        _vroView.renderDelegate = self;
        _vroView.reticle->setEnabled(true);
        [self setFrame:CGRectMake(0, 0,
                                       [[UIScreen mainScreen] bounds].size.width,
                                       [[UIScreen mainScreen] bounds].size.height)];
        [self addSubview:(UIView *)_vroView];
        
        self.currentViews = [[NSMutableArray alloc] init];
        _currentStackPosition = -1;
  }
  return self;
}

- (void) setVrModeEnabled:(BOOL)enabled{
  _vrModeEnabled = enabled;
  [_vroView setVrMode:_vrModeEnabled];
}

-(UIView *)rootVROView {
  return (UIView *)_vroView;
}

//VROComponent overrides...
- (void)insertReactSubview:(UIView *)subview atIndex:(NSInteger)atIndex {
  RCTAssert([subview isKindOfClass:[VRTScene class]], @"VRTSceneNavigator only accepts VRTScene subviews");
  VRTScene *sceneView = (VRTScene *)subview;

  [sceneView setView:_vroView];
  [self.currentViews insertObject:sceneView atIndex:atIndex];

  if (self.currentSceneIndex == atIndex){
    [self setSceneView:sceneView];
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

  VRTScene *scene = [_currentViews objectAtIndex:index];
  [self setSceneView:scene];
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


//VRORenderDelegate methods.

- (void)setupRendererWithDriver:(VRODriver *)driver {
  
}

- (void)shutdownRenderer {
  
}

- (void)renderViewDidChangeSize:(CGSize)size context:(VRORenderContext *)context {
  
}

- (void)willRenderEye:(VROEyeType)eye context:(const VRORenderContext *)context {
  
}

- (void)didRenderEye:(VROEyeType)eye context:(const VRORenderContext *)context {
  
}

- (void)reticleTapped:(VROVector3f)ray context:(const VRORenderContext *)context {
  
}

- (void)setSceneView:(VRTScene *)sceneView {
  if (_currentScene == sceneView) {
    return;
  }

  if(_currentScene == nil) {
    [_vroView setSceneController:[sceneView sceneController]];
  } else {
    [_vroView setSceneController:[sceneView sceneController] duration:1 timingFunction:VROTimingFunctionType::EaseIn];
  }
  
  VROAudioManager *audioManager = [self.bridge audioManager];
  [audioManager setCurrentScene:_currentScene];
  _currentScene = sceneView;
}

- (void)userDidRequestExitVR {
  [[NSNotificationCenter defaultCenter] postNotification:[NSNotification notificationWithName:kVRTUserRequestedExit object:nil]];
}

- (void)setApiKey:(NSString *)apiKey {
  VROViewValidApiKeyBlock completionBlock = ^(BOOL valid) {
    if (!valid) {
      RCTLogError(kVRTInvalidAPIKeyMessage);
      // fire off a notification to let the user know that the API key is invalid.
      [[NSNotificationCenter defaultCenter] postNotification:[NSNotification notificationWithName:kVRTApiKeyIsInvalid object:nil]];
    }
  };
  [_vroView validateApiKey:apiKey withCompletionBlock:completionBlock];
}

#pragma mark RVTInvalidating Delegate Methods

- (void)invalidate {
  _currentViews = nil;
  _currentScene = nil;
  _vroView = nil;
  _childViews = nil;
}

@end
