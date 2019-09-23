//
//  VRTImageAsyncLoader.m
//  React
//
//  Created by Vik Advani on 9/28/16.
//  Copyright © 2016 Viro Media. All rights reserved.
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

#import <React/RCTImageSource.h>
#import "VRTImageAsyncLoader.h"
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

- (void)cancel{
    if (_currentDownloadTask){
        [_currentDownloadTask cancel];
        _currentDownloadTask = nil;
    }
}

-(void)loadImage:(RCTImageSource *)imageSource {
    // Cancel the currently downloading task before initating a new one.
    [self cancel];
    
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
                BOOL success = (image != nil);
                if(self.delegate) {
                    [self.delegate imageLoaderDidEnd:self success:success image:image];
                }
            }
            else {
                if(self.delegate) {
                    [self.delegate imageLoaderDidEnd:self success:NO image:nil];
                }
            }
        });
    }
    else {
        NSLog(@"ERROR: Attempted to load image with unknown scheme path!");
        if(self.delegate) {
            [self.delegate imageLoaderDidEnd:self success:NO image:nil];
        }
    }
}

@end
