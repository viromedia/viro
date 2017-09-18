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

static NSString *const kVRTInvalidAPIKeyMessage = @"The given API Key is either missing or invalid! If you have not signed up for accessing Viro Media platform, please do so at www.viromedia.com. Otherwise, contact info@viromedia.com if you have a valid key and are encountering this error.";

@implementation VRTARSceneNavigator {
    id <VROView> _vroView;
    NSInteger _currentStackPosition;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        EAGLContext *context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        
        _vroView = [[VROViewAR alloc] initWithFrame:CGRectMake(0, 0,
                                                               [[UIScreen mainScreen] bounds].size.width,
                                                               [[UIScreen mainScreen] bounds].size.height) context:context];
        
        VROViewAR *viewAR = (VROViewAR *) _vroView;
        [viewAR setAutoresizingMask:UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight];
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

- (UIView *)rootVROView {
    return (UIView *)_vroView;
}

//VROComponent overrides...
- (void)insertReactSubview:(UIView *)subview atIndex:(NSInteger)atIndex {
    RCTAssert([subview isKindOfClass:[VRTARScene class]], @"VRTARNavigator only accepts VRTARScene subviews");
    VRTARScene *sceneView = (VRTARScene *)subview;
    
    [sceneView setView:_vroView];
    [self.currentViews insertObject:sceneView atIndex:atIndex];
    
    if (self.currentSceneIndex == atIndex){
        [self setSceneView:sceneView];
    }
    
    [sceneView addTraversalListenerToVROView];
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
    VRTARScene *sceneView = (VRTARScene *)subview;
    [sceneView removeTraversalListenerFromVROView];
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
