//
//  VRTManagedAnimation.h
//  ViroReact
//
//  Created by Raj Advani on 7/21/17.
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
