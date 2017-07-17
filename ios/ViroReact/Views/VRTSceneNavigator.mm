
//
//  VRTSceneNavigator.m
//  React
//
//  Created by Vik Advani on 3/11/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <ViroKit/ViroKit.h>
#import <React/RCTAssert.h>
#import <React/RCTLog.h>
#import "VRTSceneNavigator.h"
#import "VRTScene.h"
#import "VRTNotifications.h"
#import <React/RCTRootView.h>
#import <React/RCTUtils.h>
#import "VRTPerfMonitor.h"

static NSString *const kVRTInvalidAPIKeyMessage = @"The given API Key is either missing or invalid! If you have not signed up for accessing Viro Media platform, please do so at www.viromedia.com. Otherwise, contact info@viromedia.com if you have a valid key and are encountering this error.";

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
        VROViewCardboard *viewCardboard = (VROViewCardboard *) _vroView;
        [viewCardboard setAutoresizingMask:UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight];
        _vroView.renderDelegate = self;
        
        [self setFrame:CGRectMake(0, 0,
                                  [[UIScreen mainScreen] bounds].size.width,
                                  [[UIScreen mainScreen] bounds].size.height)];
        [self addSubview:(UIView *)_vroView];
        
        self.currentViews = [[NSMutableArray alloc] init];
        _currentStackPosition = -1;
        
        [bridge.perfMonitor setView:_vroView];
    }
    return self;
}

- (void) setVrModeEnabled:(BOOL)enabled{
    _vrModeEnabled = enabled;
    [_vroView setVrMode:_vrModeEnabled];
}

- (void)recenterTracking {
    VROViewCardboard *cardboardView = _vroView;
    [cardboardView recenterTracking];
}

- (UIView *)rootVROView {
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

- (void)renderViewDidChangeSize:(CGSize)size context:(VRORenderContext *)context {
    
}

- (void)willRenderEye:(VROEyeType)eye context:(const VRORenderContext *)context {
    
}

- (void)didRenderEye:(VROEyeType)eye context:(const VRORenderContext *)context {
    
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
    
    if(_currentScene == nil) {
        [_vroView setSceneController:[sceneView sceneController]];
    } else {
        [_vroView setSceneController:[sceneView sceneController] duration:1 timingFunction:VROTimingFunctionType::EaseIn];
    }
    
    _currentScene = sceneView;
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

- (void)removeFromSuperview{
    [self viewWillDisappear];
    [super removeFromSuperview];
    
    /*
     * We need to always ensure that React's root view is showing when we
     * are deallocating Viro and our renderer. This is because GVR does not
     * perform the proper cleanup on its windows: VIRO-1067
     */
    if (RCTIsMainQueue()){
        NSArray *windowArray = [UIApplication sharedApplication].windows;
        
        if (windowArray == nil || [windowArray count] == 0){
            return;
        }
        
        for (int i = 0; i < [windowArray count]; i ++){
            UIWindow *window = [windowArray objectAtIndex:i];
            if (window != nil && window.rootViewController != nil &&
                [window.rootViewController.view isKindOfClass:[RCTRootView class]]){
                [window makeKeyAndVisible];
                return;
            }
        }
    }
}

#pragma mark RCTInvalidating methods

- (void)invalidate {
    _currentViews = nil;
    _currentScene = nil;
    _vroView = nil;
    _childViews = nil;
}
@end
