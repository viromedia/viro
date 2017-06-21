//
//  VROTrackingHelper.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>

/*
 This class is currently meant to show as a proof of concept that we can fetch images from
 the AVCapture session and run them through image detection w/ OpenCV
 */

@interface VROTrackingHelper : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate>

@property (nonatomic, strong) UIImageView *overlayImageView;

- (instancetype)init;

- (void)processBuffer:(CMSampleBufferRef)newBuffer;

@end
