//
//  VRTUtils.h
//  React
//
//  Created by Vik Advani on 1/8/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RCTDefines.h"

// Populate NSarray values into regular C float array
RCT_EXTERN void populateFloatArrayFromNSArray(NSArray<NSNumber *> *arraySource, float arrayDest[], int totalCount);

// Populate NSarray values into regular C float array
RCT_EXTERN void populateIntArrayFromNSArray(NSArray<NSNumber *> *arraySource, int arrayDest[], int totalCount);

RCT_EXTERN NSURLSessionDataTask* downloadDataWithURL(NSURL *url, void (^completionBlock)(NSData *data, NSError *error));
