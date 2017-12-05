
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
    VROViewControllerGVR *_gvrController;
    NSInteger _currentStackPosition;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _gvrController = [[VROViewControllerGVR alloc] init];
        _vroView = (id<VROView>) _gvrController.view;
        
        VROViewCardboard *viewCardboard = (VROViewCardboard *) _gvrController.view;
        [viewCardboard setAutoresizingMask:UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight];
        _vroView.renderDelegate = self;
        
        [self setFrame:[UIScreen mainScreen].bounds];
        [self setAutoresizingMask:UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight];
        
        [self addSubview:viewCardboard];
        self.currentViews = [[NSMutableArray alloc] init];
        _currentStackPosition = -1;
        
        [bridge.perfMonitor setView:_vroView];
    }
    return self;
}

- (void)setVrModeEnabled:(BOOL)enabled{
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
    
    if (self.currentSceneIndex == atIndex) {
        [self setSceneView:sceneView];
        [(VROViewCardboard *)_vroView setPaused:NO];
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
