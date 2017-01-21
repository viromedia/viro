//
//  VRTImageAsyncLoader.m
//  React
//
//  Created by Vik Advani on 9/28/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTImageAsyncLoader.h"
#import "RCTImageSource.h"
#import "VRTUtils.h"


@implementation VRTImageAsyncLoader {
   NSURLSessionDataTask *_currentDownloadTask;
}

- (instancetype)initWithDelegate:(id<VRTImageAsyncLoaderEventDelegate>)delegate {
  self = [super init];
  if(self) {
    _currentDownloadTask = nil;
    self.delegate = delegate;
  }
  return self;
}

-(void)loadImage:(RCTImageSource *)imageSource {
  if (_currentDownloadTask){
    //Cancel the currently downloading task before initating a new one.
    [_currentDownloadTask cancel];
    _currentDownloadTask = nil;
  }
  
  if (self.delegate) {
    [self.delegate imageLoaderDidStart:self];
  }

  NSURL *URL = imageSource.request.URL;
  NSString *scheme = URL.scheme.lowercaseString;
  if([scheme isEqualToString:@"file"] || [scheme isEqualToString:@"http"] || [scheme isEqualToString:@"data"] ||  [scheme isEqualToString:@"https"]) {
    _currentDownloadTask = downloadDataWithURL(URL, ^(NSData *data, NSError *error) {
      if (error.code == NSURLErrorCancelled){
        // If we have cancelled this request, do nothing.
        // This is because multiple requests would be have been
        // fired as a result of the setting of props, but the
        // callback only needs to be triggered once
        // with the single final result.
        return;
      }
      
      _currentDownloadTask = nil;
      
      if (!error) {
        UIImage *image = [[UIImage alloc] initWithData:data];
        
        // Scale the Image, update texture to scene, trigger callbacks.
        if ([self scaleImage:image toRCTSourceDimensions:imageSource]) {
          if(self.delegate) {
            [self.delegate imageLoaderDidEnd:self success:YES image:image];
          }
          return;
        }
      }

      if(self.delegate) {
        [self.delegate imageLoaderDidEnd:self success:NO image:nil];
      }
    });
    return;
  } else {
    NSLog(@"ERROR: Attempted to load image with unknown scheme path!");
    if(self.delegate) {
      [self.delegate imageLoaderDidEnd:self success:NO image:nil];
    }
  }
}

/**
 * Scales the image to the size of the given image source.
 * returns true if succesful.
 **/
-(BOOL)scaleImage:(UIImage *)image toRCTSourceDimensions:(RCTImageSource *)imageSourceDimension{
  CGFloat scale = imageSourceDimension.scale;
  if (!scale && imageSourceDimension.size.width) {
    // If no scale provided, set scale to image width / source width
    scale = CGImageGetWidth(image.CGImage) / imageSourceDimension.size.width;
  }
  
  if (scale) {
    image = [UIImage imageWithCGImage:image.CGImage
                                scale:scale
                          orientation:image.imageOrientation];
  }
  
  if (!CGSizeEqualToSize(imageSourceDimension.size, CGSizeZero) &&
      !CGSizeEqualToSize(imageSourceDimension.size, image.size)) {
    RCTLogError(@"Image source size %@ does not match loaded image size %@.",
                NSStringFromCGSize(imageSourceDimension.size),
                NSStringFromCGSize(image.size));
    return NO;
  }
  return YES;
}

@end
