//
//  VROViewRecorder.h
//  ViroKit
//
//  Created by Raj Advani on 11/28/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>
#import <AVFoundation/AVFoundation.h>
#import <Photos/Photos.h>
#import <memory>

class VRODriver;
class VRORenderer;

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

/*
 Video recording and screenshot implementation for a GLKView.
 */
@interface VROViewRecorder : NSObject

- (id)initWithView:(GLKView *)view
          renderer:(std::shared_ptr<VRORenderer>)renderer
            driver:(std::shared_ptr<VRODriver>)driver;
- (void)deleteGL;

- (void)startVideoRecording:(NSString *)fileName
           saveToCameraRoll:(BOOL)saveToCamera
                 errorBlock:(VROViewRecordingErrorBlock)errorBlock;
- (void)stopVideoRecordingWithHandler:(VROViewWriteMediaFinishBlock)completionHandler;
- (void)takeScreenshot:(NSString *)fileName
      saveToCameraRoll:(BOOL)saveToCamera
 withCompletionHandler:(VROViewWriteMediaFinishBlock)completionHandler;

@end
