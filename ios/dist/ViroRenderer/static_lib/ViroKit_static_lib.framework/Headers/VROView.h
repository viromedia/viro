//
//  VROView.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/15/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

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

@protocol VROView <NSObject>

@required

@property (nonatomic, weak) IBOutlet id <VRORenderDelegate> renderDelegate;
@property (readonly, nonatomic) std::shared_ptr<VRORenderer> renderer;
@property (readwrite, nonatomic) std::shared_ptr<VROSceneController> sceneController;

- (void)setSceneController:(std::shared_ptr<VROSceneController>)sceneController;
- (void)setSceneController:(std::shared_ptr<VROSceneController>)sceneController duration:(float)seconds
            timingFunction:(VROTimingFunctionType)timingFunctionType;

- (void)setPointOfView:(std::shared_ptr<VRONode>)node;
- (void)setDebugHUDEnabled:(BOOL)enabled;
- (void)recenterTracking;

- (void)startVideoRecording:(NSString *)fileName
           saveToCameraRoll:(BOOL)saveToCamera
                 errorBlock:(VROViewRecordingErrorBlock)errorBlock;

- (void)startVideoRecording:(NSString *)fileName
              withWatermark:(UIImage *)watermarkImage
                  withFrame:(CGRect)watermarkFrame
           saveToCameraRoll:(BOOL)saveToCamera
                 errorBlock:(VROViewRecordingErrorBlock)errorBlock;

- (void)startVideoRecording:(NSString *)fileName
                    gifFile:(NSString *)gifFile
              withWatermark:(UIImage *)watermarkImage
                  withFrame:(CGRect)watermarkFrame
           saveToCameraRoll:(BOOL)saveToCamera
                 errorBlock:(VROViewRecordingErrorBlock)errorBlock;

- (void)stopVideoRecordingWithHandler:(VROViewWriteMediaFinishBlock)completionHandler;
- (void)stopVideoRecordingWithHandler:(VROViewWriteMediaFinishBlock)completionHandler mergeAudioTrack:(NSURL *)audioPath;
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
