//
//  VROView.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/15/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "VRORenderDelegate.h"
#import "VROQuaternion.h"
#import "VROCamera.h"
#import <memory>

class VROSceneController;
class VROScene;
class VROReticle;
class VRONode;
class VROFrameSynchronizer;
enum class VROTimingFunctionType;

typedef void (^VROViewValidApiKeyBlock)(BOOL);

/*
 These blocks are used for video recording and taking screenshot.
 */
typedef void (^VROViewRecordingErrorBlock)(NSInteger errorCode);
typedef void (^VROViewWriteMediaFinishBlock)(BOOL success, NSURL *filePath, NSInteger errorCode);

static NSInteger const kVROViewErrorNone = -1;
static NSInteger const kVROViewErrorUnknown = 0;
static NSInteger const kVROViewErrorNoPermissions = 1;
static NSInteger const kVROViewErrorInitialization = 2;
static NSInteger const kVROViewErrorWriteToFile = 3;
static NSInteger const kVROViewErrorAlreadyRunning = 4;
static NSInteger const kVROViewErrorAlreadyStopped = 5;


static NSString *const kVROViewTempMediaDirectory = @"viro_media";
static NSString *const kVROViewAudioSuffix = @".m4a";
static NSString *const kVROViewImageSuffix = @".png";
static NSString *const kVROViewTempVideoSuffix = @"-temp.mp4";
static NSString *const kVROViewVideoSuffix = @".mp4";

@protocol VROView <NSObject>

@required

@property (nonatomic, weak) IBOutlet id <VRORenderDelegate> renderDelegate;
@property (readwrite, nonatomic) std::shared_ptr<VROSceneController> sceneController;

- (void)setSceneController:(std::shared_ptr<VROSceneController>)sceneController;
- (void)setSceneController:(std::shared_ptr<VROSceneController>)sceneController duration:(float)seconds
            timingFunction:(VROTimingFunctionType)timingFunctionType;

- (void)setPointOfView:(std::shared_ptr<VRONode>)node;
- (void)validateApiKey:(NSString *)apiKey withCompletionBlock:(VROViewValidApiKeyBlock)completionBlock;
- (void)setDebugHUDEnabled:(BOOL)enabled;
- (void)recenterTracking;

- (void)startVideoRecording:(NSString *)fileName
           saveToCameraRoll:(BOOL)saveToCamera
                 errorBlock:(VROViewRecordingErrorBlock)errorBlock;
- (void)stopVideoRecordingWithHandler:(VROViewWriteMediaFinishBlock)completionHandler;
- (void)takeScreenshot:(NSString *)fileName
      saveToCameraRoll:(BOOL)saveToCamera
 withCompletionHandler:(VROViewWriteMediaFinishBlock)completionHandler;

- (NSString *)getPlatform;
- (NSString *)getHeadset;
- (NSString *)getController;

/*
 Calling setVrMode allows switching to and from VR mode.
 When set to NO, it transitions back to pre-VR (mono) mode.
 When set to YES, we set thie view into a full VR mode
 */
- (void)setVrMode:(BOOL)enabled;

- (std::shared_ptr<VROFrameSynchronizer>)frameSynchronizer;

@end
