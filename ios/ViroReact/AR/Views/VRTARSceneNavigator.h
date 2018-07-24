//
//  VRTARSceneNavigator.h
//  ViroReact
//
//  Created by Andy Chu on 6/12/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <React/RCTBridge.h>
#import <React/RCTInvalidating.h>
#import "VRTNode.h"

@class VRTScene;

@interface VRTARSceneNavigator : VRTView<VRORenderDelegate, RCTInvalidating>

@property (nonatomic, assign) NSInteger currentSceneIndex;
@property (nonatomic, readwrite, strong) NSMutableArray<VRTScene *> *currentViews;
@property (readwrite, nonatomic) VRTScene *currentScene;
@property (nonatomic, copy) NSString *apiKey;
@property (nonatomic, copy) NSString *worldAlignment;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onExitViro;
@property (nonatomic, assign) BOOL autofocus;
@property (nonatomic, copy) NSString *videoQuality;
@property (nonatomic, readwrite) BOOL hdrEnabled;
@property (nonatomic, readwrite) BOOL pbrEnabled;
@property (nonatomic, readwrite) BOOL bloomEnabled;
@property (nonatomic, readwrite) BOOL shadowsEnabled;

- (instancetype)initWithBridge:(RCTBridge *)bridge;
- (void)insertReactSubview:(UIView *)subview atIndex:(NSInteger)atIndex;
- (void)removeReactSubview:(UIView *)subview;
- (NSArray *)reactSubviews;
- (UIView *)reactSuperview;
- (UIView *)rootVROView;
- (void)invalidate;

- (void)startVideoRecording:(NSString *)fileName
           saveToCameraRoll:(BOOL)saveToCameraRoll
                    onError:(RCTResponseSenderBlock)onError;

- (void)stopVideoRecordingWithHandler:(VROViewWriteMediaFinishBlock)completionHandler;

- (void)takeScreenshot:(NSString *)fileName
      saveToCameraRoll:(BOOL)saveToCameraRoll
     completionHandler:(VROViewWriteMediaFinishBlock)completionHandler;
    

@end
