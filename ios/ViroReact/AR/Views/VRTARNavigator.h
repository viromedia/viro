//
//  VRTARNavigator.h
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

@interface VRTARNavigator : VRTView<VRORenderDelegate, RCTInvalidating>

@property (nonatomic, assign) NSInteger currentSceneIndex;
@property (nonatomic, readwrite, strong) NSMutableArray<VRTScene *> *currentViews;
@property (readwrite, nonatomic) VRTScene *currentScene;
@property (nonatomic, copy) NSString *apiKey;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onExitViro;

- (instancetype)initWithBridge:(RCTBridge *)bridge;
- (void)insertReactSubview:(UIView *)subview atIndex:(NSInteger)atIndex;
- (void)removeReactSubview:(UIView *)subview;
- (NSArray *)reactSubviews;
- (UIView *)reactSuperview;
- (UIView *)rootVROView;
- (void)invalidate;

- (void)startVideoRecording:(NSString *)fileName
           saveToCameraRoll:(BOOL)saveToCameraRoll;

- (void)stopVideoRecordingWithHandler:(VROViewWriteMediaFinishBlock)completionHandler;

- (void)takeScreenshot:(NSString *)fileName
      saveToCameraRoll:(BOOL)saveToCameraRoll
     completionHandler:(VROViewWriteMediaFinishBlock)completionHandler;
    

@end
