//
//  VRTUtils.m
//  React
//
//  Created by Vik Advani on 1/8/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTUtils.h"
#import <React/RCTUtils.h>


// Populate NSarray values into regular C float array
void populateFloatArrayFromNSArray(NSArray<NSNumber *> *arraySource, float arrayDest[], int totalCount){
    int index =0;
    for (NSNumber *value in arraySource) {
        arrayDest[index] = [value floatValue];
        index++;
        if(index >=totalCount){
            break;
        }
    }
}

// Populate NSarray values into regular C int array
void populateIntArrayFromNSArray(NSArray<NSNumber *> *arraySource, int arrayDest[], int totalCount){
    int index =0;
    for (NSNumber *value in arraySource) {
        arrayDest[index] = [value intValue];
        index++;
        if(index >=totalCount){
            break;
        }
    }
}

// Request data from a URL from a background thread and invoke a block on the main UI thread on completion.
// Default timeout is 30 seconds on the NSMutableURLRequest, after a timeout, the block is called with an error.
NSURLSessionDataTask* downloadDataWithURL(NSURL *url, void (^completionBlock)(NSData *data, NSError *error)){
    NSURLSessionConfiguration *sessionConfig = [NSURLSessionConfiguration defaultSessionConfiguration];
    sessionConfig.timeoutIntervalForRequest = 30;
    NSURLSession *downloadSession = [NSURLSession sessionWithConfiguration: sessionConfig];
    NSURLSessionDataTask * downloadTask = [downloadSession dataTaskWithURL:url
                                                         completionHandler:^(NSData *data, NSURLResponse *response, NSError *error){
                                                             completionBlock(data, error);
                                                         }];
    [downloadTask resume];
    return downloadTask;
}
