//
//  VROViewRecorder.h
//  ViroKit
//
//  Created by Raj Advani on 11/28/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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
#import <GLKit/GLKit.h>
#import <AVFoundation/AVFoundation.h>
#import <Photos/Photos.h>
#import <memory>
#import "VRORenderToTextureDelegate.h"

class VRODriver;
class VRORenderer;
class VROTexture;
class VRORenderTarget;
@class VROViewRecorder;

typedef void (^VROViewRecordingErrorBlock)(NSInteger errorCode);
typedef void (^VROViewWriteMediaFinishBlock)(BOOL success, NSURL *filePath, NSURL *gifPath, NSInteger errorCode);

static NSInteger const kVROViewErrorNone = -1;
static NSInteger const kVROViewErrorUnknown = 0;
static NSInteger const kVROViewErrorNoPermissions = 1;
static NSInteger const kVROViewErrorInitialization = 2;
static NSInteger const kVROViewErrorWriteToFile = 3;
static NSInteger const kVROViewErrorAlreadyRunning = 4;
static NSInteger const kVROViewErrorAlreadyStopped = 5;
static NSInteger const kVROViewErrorWriteGifToFile = 6;
static NSInteger const kVROViewErrorAudioFailure = 7;

static NSString *const kVROViewTempMediaDirectory = @"viro_media";
static NSString *const kVROViewAudioSuffix = @".m4a";
static NSString *const kVROViewImageSuffix = @".png";
static NSString *const kVROViewTempVideoSuffix = @"-temp.mp4";
static NSString *const kVROViewVideoSuffix = @".mp4";
static NSString *const kVROViewGifSuffix = @".gif";


class VROViewRecorderRTTDelegate : public VRORenderToTextureDelegate {
public:
    VROViewRecorderRTTDelegate(VROViewRecorder *recorder,
                               std::shared_ptr<VROTexture> texture,
                               std::shared_ptr<VRORenderer> renderer,
                               std::shared_ptr<VRODriver> driver);
    virtual ~VROViewRecorderRTTDelegate() {}
    
    void didRenderFrame(std::shared_ptr<VRORenderTarget> target,
                        std::shared_ptr<VRODriver> driver);
    
private:
    __weak VROViewRecorder *_recorder;
    std::shared_ptr<VROTexture> _texture;
    std::weak_ptr<VRORenderer> _renderer;
    std::shared_ptr<VRORenderTarget> _renderToTextureTarget;
};

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
- (void)lockPixelBuffer;
- (void)setUseMicrophone:(BOOL)useMicrophone;

/*
 Manually overrides video recording parameters with a different width and
 height, instead of using the dimensions of the underlying GLView.
 */
- (void)setRecorderWidth:(int)width
                  height:(int)height;
@end
