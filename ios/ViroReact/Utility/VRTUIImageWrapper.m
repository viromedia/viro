//
//  VRTUIImageWrapper.m
//  ViroReact
//
//  Created by Vik Advani on 3/20/17.
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
//

#import "VRTUIImageWrapper.h"
#import <React/RCTUtils.h>
#import <React/RCTImageSource.h>


@implementation VRTUIImageWrapper
@end

@implementation RCTConvert (VRTUIImageWrapper)

// Copied over from +(UIImage)RCTConvert in React, modified method to allow for https and http urls that
// aren not tied to the packager.
+ (VRTUIImageWrapper *)VRTUIImageWrapper:(id)json {
    if (!json) {
        return nil;
    }
    
    RCTImageSource *imageSource = [RCTConvert RCTImageSource:json];
    if (!imageSource) {
        return nil;
    }
    
    __block UIImage *image;
    if (!RCTIsMainQueue()) {
        // It seems that none of the UIImage loading methods can be guaranteed
        // thread safe, so we'll pick the lesser of two evils here and block rather
        // than run the risk of crashing
        RCTLogWarn(@"Calling [RCTConvert UIImage:] on a background thread is not recommended");
        dispatch_sync(dispatch_get_main_queue(), ^{
            VRTUIImageWrapper *imageWrapper = [RCTConvert VRTUIImageWrapper:json];
            image = imageWrapper.image;
        });
        return image;
    }
    
    NSURL *URL = imageSource.request.URL;
    NSString *scheme = URL.scheme.lowercaseString;
    if ([scheme isEqualToString:@"file"]) {
        NSString *assetName = RCTBundlePathForURL(URL);
        image = assetName ? [UIImage imageNamed:assetName] : nil;
        if (!image) {
            // Attempt to load from the file system
            NSString *filePath = URL.path;
            if (filePath.pathExtension.length == 0) {
                filePath = [filePath stringByAppendingPathExtension:@"png"];
            }
            image = [UIImage imageWithContentsOfFile:filePath];
            if (!image) {
                RCTLogConvertError(json, @"an image. File not found.");
            }
        }
    } else if ([scheme isEqualToString:@"data"]) {
        image = [UIImage imageWithData:[NSData dataWithContentsOfURL:URL]];
    } else if ([scheme hasPrefix:@"http"]) {
        image = [UIImage imageWithData:[NSData dataWithContentsOfURL:URL]];
    } else {
        RCTLogConvertError(json, @"an image. Only local files or data URIs are supported.");
        return nil;
    }
    
    CGFloat scale = imageSource.scale;
    if (!scale && imageSource.size.width) {
        // If no scale provided, set scale to image width / source width
        scale = CGImageGetWidth(image.CGImage) / imageSource.size.width;
    }
    
    if (scale) {
        image = [UIImage imageWithCGImage:image.CGImage
                                    scale:scale
                              orientation:image.imageOrientation];
    }
    
    if (!CGSizeEqualToSize(imageSource.size, CGSizeZero) &&
        !CGSizeEqualToSize(imageSource.size, image.size)) {
        RCTLogError(@"Image source %@ size %@ does not match loaded image size %@.",
                    URL.path.lastPathComponent,
                    NSStringFromCGSize(imageSource.size),
                    NSStringFromCGSize(image.size));
    }
    
    VRTUIImageWrapper *uiImageWrapper = [[VRTUIImageWrapper alloc] init];
    uiImageWrapper.image = image;
    return uiImageWrapper;
}

@end
