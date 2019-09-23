//
//  VRTPhotoLibraryAsyncLoader.m
//  ViroReact
//
//  Created by vik.advani on 8/25/17.
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

#import <Foundation/Foundation.h>

/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import "VRTPhotoLibraryAsyncLoader.h"

#import <Photos/Photos.h>

#import <React/RCTUtils.h>

@implementation VRTPhotoLibraryAsyncLoader


#pragma mark - RCTImageLoader

- (BOOL)canLoadImageURL:(NSURL *)requestURL
{
    if (![PHAsset class]) {
        return NO;
    }
    return [requestURL.scheme caseInsensitiveCompare:@"assets-library"] == NSOrderedSame ||
    [requestURL.scheme caseInsensitiveCompare:@"ph"] == NSOrderedSame;
}

- (instancetype)initWithDelegate:(id<VRTImageAsyncLoaderEventDelegate>)delegate {
    self = [super init];
    if(self) {
        self.delegate = delegate;
    }
    return self;
}

-(void)loadImage:(RCTImageSource *)imageSource 
{
    // Using PhotoKit for iOS 8+
    // The 'ph://' prefix is used by FBMediaKit to differentiate between
    // assets-library. It is prepended to the local ID so that it is in the
    // form of an, NSURL which is what assets-library uses.
    NSURL *imageURL = imageSource.request.URL;
    NSString *assetID = @"";
    PHFetchResult *results;
    if ([imageURL.scheme caseInsensitiveCompare:@"assets-library"] == NSOrderedSame) {
        assetID = [imageURL absoluteString];
        results = [PHAsset fetchAssetsWithALAssetURLs:@[imageURL] options:nil];
    } else {
        assetID = [imageURL.absoluteString substringFromIndex:@"ph://".length];
        results = [PHAsset fetchAssetsWithLocalIdentifiers:@[assetID] options:nil];
    }
    if (results.count == 0) {
        NSString *errorText = [NSString stringWithFormat:@"Failed to fetch PHAsset with local identifier %@ with no error message.", assetID];
        if(self.delegate) {
            [self.delegate imageLoaderDidEnd:self success:false image:nil];
        }
        return;
    }
    
    PHAsset *asset = [results firstObject];
    PHImageRequestOptions *imageOptions = [PHImageRequestOptions new];
    
    // Allow PhotoKit to fetch images from iCloud
    imageOptions.networkAccessAllowed = YES;
    
    //VA: TODO, perhaps want this working at a later date.
    //if (progressHandler) {
    //    imageOptions.progressHandler = ^(double progress, NSError *error, BOOL *stop, NSDictionary<NSString *, id> *info) {
    //        static const double multiplier = 1e6;
    //        progressHandler(progress * multiplier, multiplier);
    //    };
    // }
    
    // Note: PhotoKit defaults to a deliveryMode of PHImageRequestOptionsDeliveryModeOpportunistic
    // which means it may call back multiple times - we probably don't want that
    imageOptions.deliveryMode = PHImageRequestOptionsDeliveryModeHighQualityFormat;
    
    BOOL useMaximumSize = true; //CGSizeEqualToSize(size, CGSizeZero);
    CGSize targetSize;
    if (useMaximumSize) {
        targetSize = PHImageManagerMaximumSize;
        imageOptions.resizeMode = PHImageRequestOptionsResizeModeNone;
    }
    //else {
    //    targetSize = CGSizeApplyAffineTransform(size, CGAffineTransformMakeScale(scale, scale));
    //    imageOptions.resizeMode = PHImageRequestOptionsResizeModeFast;
    //}
    
    PHImageContentMode contentMode = PHImageContentModeAspectFill;
    
    PHImageRequestID requestID =
    [[PHImageManager defaultManager] requestImageForAsset:asset
                                               targetSize:targetSize
                                              contentMode:contentMode
                                                  options:imageOptions
                                            resultHandler:^(UIImage *result, NSDictionary<NSString *, id> *info) {
                                                if (result) {
                                                    if(self.delegate) {
                                                        [self.delegate imageLoaderDidEnd:self success:true image:result];
                                                    }
                                                   
                                                } else {
                                                    if(self.delegate) {
                                                        [self.delegate imageLoaderDidEnd:self success:false image:nil];
                                                    }
                                                }
                                            }];
    
}

@end
