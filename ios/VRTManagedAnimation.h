//
//  VRTManagedAnimation.h
//  ViroReact
//
//  Created by Raj Advani on 7/21/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ViroKit/ViroKit.h>
#import <React/RCTComponent.h>
#import <memory>

class VROExecutableAnimation;

@interface VRTManagedAnimation : NSObject

@property (nonatomic, assign) float delay;
@property (nonatomic, assign) float durationOverride;
@property (nonatomic, assign) BOOL loop;
@property (nonatomic, assign) BOOL run;
@property (nonatomic, assign) BOOL interruptible;
@property (nonatomic, readwrite) std::weak_ptr<VRONode> node;
@property (nonatomic, copy) RCTDirectEventBlock onStart;
@property (nonatomic, copy) RCTDirectEventBlock onFinish;

/*
 Implemented by subclasses: load the animation to be run. This is 
 invoked just before each time we start the animation, so that the
 animation may be updated between loops.
 */
- (std::shared_ptr<VROExecutableAnimation>)loadAnimation;

/*
 Invoke after updating any animation properties. Changes the running
 state of the animation (e.g. if run is set to true, and the animation
 is not running, will start the animation).
 */
- (void)updateAnimation;

/*
 Parse the properties of this animation from the given dictionary.
 This will populate delay, loop, run, onStart, and onFinish. The
 node and the animation must be set manually.
 */
- (void)parseFromDictionary:(NSDictionary *)dictionary;

@end
