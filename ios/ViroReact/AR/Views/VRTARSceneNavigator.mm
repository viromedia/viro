//
//  VRTARSceneNavigator.mm
//  ViroReact
//
//  Created by Andy Chu on 6/12/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <ViroKit/ViroKit.h>
#import "VRTARSceneNavigator.h"
#import <React/RCTAssert.h>
#import <React/RCTLog.h>
#import "VRTARScene.h"
#import "VRTNotifications.h"
#import <React/RCTRootView.h>
#import <React/RCTUtils.h>
#import "VRTPerfMonitor.h"
#import "VRTMaterialManager.h"

static NSString *const kVRTInvalidAPIKeyMessage = @"The given API Key is either missing or invalid! If you have not signed up for accessing Viro Media platform, please do so at www.viromedia.com. Otherwise, contact info@viromedia.com if you have a valid key and are encountering this error.";

@implementation VRTARSceneNavigator {
    id <VROView> _vroView;
    NSInteger _currentStackPosition;
    RCTBridge *_bridge;
    VROVideoQuality _vroVideoQuality;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        // Load materials; must be done each time we have a new context (e.g. after
        // the EGL context is created by the VROViewAR
        VRTMaterialManager *materialManager = [bridge materialManager];
        [materialManager reloadMaterials];
        
        [self setFrame:CGRectMake(0, 0,
                                  [[UIScreen mainScreen] bounds].size.width,
                                  [[UIScreen mainScreen] bounds].size.height)];
        self.currentViews = [[NSMutableArray alloc] init];
        _currentStackPosition = -1;

        _bridge = bridge;
        _autofocus = YES;
        _vroVideoQuality = VROVideoQuality::High;
    }
    return self;
}

- (void)setAutofocus:(BOOL)autofocus {
    _autofocus = autofocus;
    if (_vroView) {
        VROViewAR *viewAR = (VROViewAR *) _vroView;
        std::shared_ptr<VROARSession> arSession = [viewAR getARSession];
        arSession->setAutofocus(_autofocus);
    }
}

- (void)setVideoQuality:(NSString *)videoQuality {
    _videoQuality = videoQuality;
    if ([videoQuality caseInsensitiveCompare:@"Low"] == NSOrderedSame) {
        _vroVideoQuality = VROVideoQuality::Low;
    } else {
        _vroVideoQuality = VROVideoQuality::High;
    }
    if (_vroView) {
        VROViewAR *viewAR = (VROViewAR *) _vroView;
        std::shared_ptr<VROARSession> arSession = [viewAR getARSession];
        arSession->setVideoQuality(_vroVideoQuality);
    }
}


- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    // if we haven't created the VROView, then create it now that
    // all the props have been set.
    if (!_vroView) {
        
        VROWorldAlignment worldAlignment = VROWorldAlignment::Gravity;
        if (_worldAlignment) {
            if ([_worldAlignment caseInsensitiveCompare:@"Gravity"] == NSOrderedSame) {
                worldAlignment = VROWorldAlignment::Gravity;
            } else if ([_worldAlignment caseInsensitiveCompare:@"GravityAndHeading"] == NSOrderedSame) {
                worldAlignment = VROWorldAlignment::GravityAndHeading;
            } else if ([_worldAlignment caseInsensitiveCompare:@"Camera"] == NSOrderedSame) {
                worldAlignment = VROWorldAlignment::Camera;
            }
        }
        
        EAGLContext *context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        VRORendererConfiguration config;
        _vroView = [[VROViewAR alloc] initWithFrame:CGRectMake(0, 0,
                                                               [[UIScreen mainScreen] bounds].size.width,
                                                               [[UIScreen mainScreen] bounds].size.height)
                                             config:config
                                            context:context
                                     worldAlignment:worldAlignment];

        VROViewAR *viewAR = (VROViewAR *) _vroView;
        [viewAR setAutoresizingMask:UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight];
        _vroView.renderDelegate = self;
        
        [self addSubview:(UIView *)_vroView];

        [_bridge.perfMonitor setView:_vroView];

        // reset the API key now that _vroView has been created.
        [self setApiKey:_apiKey];

        // set the scene if it was set before this view was created (not likely)
        if (_currentScene) {
            [_vroView setSceneController:[_currentScene sceneController]];
        }

        std::shared_ptr<VROARSession> arSession = [viewAR getARSession];
        arSession->setAutofocus(_autofocus);
        arSession->setVideoQuality(_vroVideoQuality);
    }
}

- (UIView *)rootVROView {
    return (UIView *)_vroView;
}

//VROComponent overrides...
- (void)insertReactSubview:(UIView *)subview atIndex:(NSInteger)atIndex {
    RCTAssert([subview isKindOfClass:[VRTARScene class]], @"VRTARNavigator only accepts VRTARScene subviews");
    [super insertReactSubview:subview atIndex:atIndex];
    
    VRTARScene *sceneView = (VRTARScene *)subview;
    
    [sceneView setView:_vroView];
    [self.currentViews insertObject:sceneView atIndex:atIndex];
    
    if (self.currentSceneIndex == atIndex){
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
    VRTARScene *sceneView = (VRTARScene *)subview;
    [self.currentViews removeObject:sceneView];
    [super removeReactSubview:subview];
}

- (NSArray *)reactSubviews {
    return self.currentViews;
}

- (UIView *)reactSuperview {
    return nil;
}

#pragma mark - VRORenderDelegate methods

- (void)setupRendererWithDriver:(std::shared_ptr<VRODriver>)driver {
    
}

- (void)startVideoRecording:(NSString *)fileName
           saveToCameraRoll:(BOOL)saveToCameraRoll
                    onError:(RCTResponseSenderBlock)onError {
    VROViewAR *viewAR = (VROViewAR *) _vroView;
    [viewAR startVideoRecording:fileName saveToCameraRoll:saveToCameraRoll errorBlock:^(NSInteger errorCode) {
        onError(@[@(errorCode)]);
    }];
}

- (void)stopVideoRecordingWithHandler:(VROViewWriteMediaFinishBlock)completionHandler {
    VROViewAR *viewAR = (VROViewAR *) _vroView;
    [viewAR stopVideoRecordingWithHandler:completionHandler];
}

- (void)takeScreenshot:(NSString *)fileName
      saveToCameraRoll:(BOOL)saveToCameraRoll
     completionHandler:(VROViewWriteMediaFinishBlock)completionHandler {
    VROViewAR *viewAR = (VROViewAR *) _vroView;
    [viewAR takeScreenshot:fileName saveToCameraRoll:saveToCameraRoll withCompletionHandler:completionHandler];
    
}

- (void)setSceneView:(VRTScene *)sceneView {
    if (_currentScene == sceneView) {
        return;
    }

    if (_vroView) {
        if (_currentScene == nil) {
            [_vroView setSceneController:[sceneView sceneController]];
        } else {
            [_vroView setSceneController:[sceneView sceneController] duration:1 timingFunction:VROTimingFunctionType::EaseIn];
        }
    }

    _currentScene = sceneView;
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

- (void)removeFromSuperview{
    [self parentDidDisappear];
    if (_vroView) {
        VROViewAR *viewAR = (VROViewAR *)_vroView;
        [viewAR deleteGL];
    }

    [super removeFromSuperview];
}

#pragma mark RCTInvalidating methods

- (void)invalidate {
    if (_vroView) {
        // pause the view before removing it.
        VROViewAR *viewAR = (VROViewAR *)_vroView;
        [viewAR setPaused:YES];
    }
    //NOTE: DO NOT NULL OUT _currentViews here, that will cause a memory leak and prevent child views from being released.
    _currentScene = nil;
    _vroView = nil;
    _childViews = nil;
}
@end
