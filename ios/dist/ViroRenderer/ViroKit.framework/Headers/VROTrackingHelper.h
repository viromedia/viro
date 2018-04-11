//
//  VROTrackingHelper.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#if ENABLE_OPENCV

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>
#import "opencv2/imgcodecs/ios.h"
#import "VROARCamera.h"
#import "VROARImageTracker.h"

@interface VROTrackingHelperOutput : NSObject

- (instancetype)initWithTrackerOutput:(VROARImageTrackerOutput)output withImage:(UIImage *)outputImage;

- (VROARImageTrackerOutput)getImageTrackerOutput;
- (UIImage *)getOutputImage;

@end

/*
 This class is a helper class that takes the given PixelBuffer and feeds it into a
 VROARImageTracker that has been preconfigured with a target image
 */
@interface VROTrackingHelper : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate>

@property (nonatomic, strong) UIImageView *overlayImageView;

- (instancetype)init;

/*
 This function runs the same test as ViroActivity's testFindInScreenshot on Android. Useful
 for ensuring similar performance on both platforms.
 */
- (void)findInScreenshot;

- (void)setIntrinsics:(float *)intrinsics;

- (void)processBuffer:(CMSampleBufferRef)newBuffer;

- (void)processPixelBufferRef:(CVPixelBufferRef)pixelBuffer
                     forceRun:(BOOL)forceRun
                       camera:(std::shared_ptr<VROARCamera>)camera
                   completion:(void (^)(VROTrackingHelperOutput *output))completionHandler;

- (cv::Mat)matFromYCbCrBuffer:(CVImageBufferRef)buffer;

/*
 Sets tracking on/off depending on the given bool.
 */
- (void)toggleTracking:(BOOL)tracking;

/*
 Toggles tracking on/off depending on existing state.
 */
- (BOOL)toggleTracking;


@end

#endif /* ENABLE_OPENCV */
