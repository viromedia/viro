//
//  VRTARTrackingTargetsModule.h
//  ViroReact
//
//  Created by Andy Chu on 2/1/18.
//  Copyright © 2018 Viro Media. All rights reserved.
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
#import <ViroKit/ViroKit.h>
#import <React/RCTBridgeModule.h>
#import <React/RCTBridge.h>
#import <React/RCTImageSource.h>

// ----- VRTARImageTargetPromise -----

/*
 VRTARImageTargetPromiseCompletion block called with the results of a Promise's fetch with
 the NSString key and the associated VROARImageTarget (or nullptr if there's an error).
 */
typedef void (^ VRTARImageTargetPromiseCompletion)(NSString *, std::shared_ptr<VROARImageTarget>);


/*
 This class encompasses the logic required for fetching the data and creating a
 VROARImageTarget object that allows execution to continue.
 */
@interface VRTARImageTargetPromise : NSObject

@property (nonatomic, assign) BOOL ready;

- (instancetype)initWithKey:(NSString *)key
                     source:(RCTImageSource *)source
                orientation:(VROImageOrientation)orientation
              physicalWidth:(float)physicalWidth;

/*
 This tells the VRTARImageTargetPromise to fetch the target. The module should automatically
 call this after creating the promise.
 */
- (void)fetch;

/*
 To retrieve the results of this promise, "wait" on it by passing in a completion
 block which will either be called immediately if the data is available or later
 after the fetch completes.
 */
- (void)wait:(VRTARImageTargetPromiseCompletion)finishBlock;

@end

// ----- VRTARObjectTargetPromise -----

/*
 VRTARObjectTargetPromiseCompletion block called with the results of a Promise's fetch with
 the NSString key and the associated VROARObjectTarget (or nullptr if there's an error).
 */
typedef void (^ VRTARObjectTargetPromiseCompletion)(NSString *, std::shared_ptr<VROARObjectTarget>);


/*
 This class encompasses the logic required for fetching the data and creating a
 VROARObjectTarget object that allows execution to continue.
 */
@interface VRTARObjectTargetPromise : NSObject

@property (nonatomic, assign) BOOL ready;

- (instancetype)initWithKey:(NSString *)key
                     source:(RCTImageSource *)source;

/*
 This tells the VRTARObjectTargetPromise to fetch the target. The module should automatically
 call this after creating the promise.
 */
- (void)fetch;

/*
 To retrieve the results of this promise, "wait" on it by passing in a completion
 block which will either be called immediately if the data is available or later
 after the fetch completes.
 */
- (void)wait:(VRTARObjectTargetPromiseCompletion)finishBlock;

@end

/*
 The VRTARTrackingTargetsModule allows the user to create and delete tracking targets to be
 used with AR components that accept them.
 */
@interface VRTARTrackingTargetsModule : NSObject <RCTBridgeModule>

/*
 Returns the VRTARImageTargetPromise for the given NSString name. Returns nil if nothing is there.
 */
- (VRTARImageTargetPromise *)getARImageTargetPromise:(NSString *)name;

/*
 Returns the VRTARObjectTargetPromise for the given NSString name. Returns nil if nothing is there.
 */
- (VRTARObjectTargetPromise *)getARObjectTargetPromise:(NSString *)name;

@end

/*
 RCTBridge category that allows us to fet the VRTARTrackingTargetsModule from the bridge object
 */
@interface RCTBridge (VRTARTrackingTargetsModule)

- (VRTARTrackingTargetsModule *)trackingTargetsModule;

@end
