//
//  VRTARTrackingTargetsModule.h
//  ViroReact
//
//  Created by Andy Chu on 2/1/18.
//  Copyright © 2018 Viro Media. All rights reserved.
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
