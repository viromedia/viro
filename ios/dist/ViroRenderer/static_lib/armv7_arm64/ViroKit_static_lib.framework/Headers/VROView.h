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
#import "VROViewRecorder.h"
#import <memory>

class VROSceneController;
class VROScene;
class VROReticle;
class VRONode;
class VROFrameSynchronizer;
enum class VROTimingFunctionType;

typedef void (^VROViewValidApiKeyBlock)(BOOL);

@protocol VROView <NSObject>

@required

@property (nonatomic, weak) IBOutlet id <VRORenderDelegate> renderDelegate;
@property (readonly, nonatomic) std::shared_ptr<VRORenderer> renderer;
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

- (BOOL)setShadowsEnabled:(BOOL)enabled;
- (BOOL)setHDREnabled:(BOOL)enabled;
- (BOOL)setPBREnabled:(BOOL)enabled;
- (BOOL)setBloomEnabled:(BOOL)enabled;

- (VROVector3f)unprojectPoint:(VROVector3f)point;
- (VROVector3f)projectPoint:(VROVector3f)point;

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
