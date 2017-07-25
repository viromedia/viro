//
//  VRTManagedAnimation.m
//  ViroReact
//
//  Created by Raj Advani on 7/21/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import "VRTManagedAnimation.h"

enum class VRTAnimationState {
    Scheduled,
    Running,
    Paused,
    Terminated
};

@interface VRTManagedAnimation ()

@property (readwrite, nonatomic) VRTAnimationState state;
@property (readwrite, nonatomic) std::shared_ptr<VROExecutableAnimation> executableAnimation;

@end

@implementation VRTManagedAnimation

- (id)init {
    self = [super init];
    if (self) {
        self.delay = -1.0f;
        self.loop = false;
        self.run = true;
        self.state = VRTAnimationState::Terminated;
    }
    return self;
}

- (void)dealloc {
    // Cleanup any transactions that were paused
    if (self.executableAnimation) {
        self.executableAnimation->terminate();
        self.executableAnimation.reset();
    }
}

- (void)setAnimation:(std::shared_ptr<VROExecutableAnimation>)animation {
    if (self.executableAnimation) {
        self.executableAnimation->terminate();
    }
    
    self.state = VRTAnimationState::Terminated;
    // currently set animation.
    _executableAnimation = animation;
}

- (void)parseFromDictionary:(NSDictionary *)dictionary {
    id delayValue = [dictionary objectForKey:@"delay"];
    if (delayValue != nil) {
        self.delay = [delayValue floatValue];
    }
    else {
        self.delay = -1;
    }
    
    self.loop = [[dictionary objectForKey:@"loop"] boolValue];
    self.run = [[dictionary objectForKey:@"run"] boolValue];
}

- (void)updateAnimation {
    if (self.run) {
        [self playAnimation];
    }
    else {
        [self pauseAnimation];
    }
}

/*
 Plays the animation. If the animation is paused, resumes the animation.
 Otherwise starts a new animation if is not running. If there is a delay
 associated with this VRTAnimatedComponent, we start a new animation with the
 given delay.
 */
- (void)playAnimation {
    if (self.state == VRTAnimationState::Paused) {
        self.executableAnimation->resume();
        self.state = VRTAnimationState::Running;
    }
    else if (self.state == VRTAnimationState::Terminated) {
        self.state = VRTAnimationState::Scheduled;
        if(self.delay <= 0) {
            // start animation right away if there is no delay.
            [self startAnimation];
        }else {
            [self performSelector:@selector(startAnimation) withObject:self afterDelay:self.delay / 1000.0];
        }
    }
}

- (void)pauseAnimation {
    if (self.state == VRTAnimationState::Running) {
        self.executableAnimation->pause();
        self.state = VRTAnimationState::Paused;
    }
    else if (self.state == VRTAnimationState::Scheduled) {
        [NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(startAnimation) object:self];
        self.state = VRTAnimationState::Terminated;
        self.executableAnimation.reset();
    }
}

- (void)startAnimation {
    if (self.state != VRTAnimationState::Scheduled) {
        NSLog(@"Aborted starting new animation, was no longer scheduled");
        return;
    }
    
    if (!self.executableAnimation) {
        NSLog(@"Cannot start animation: no animation loaded!");
        self.state = VRTAnimationState::Terminated;
        return;
    }
    
    std::shared_ptr<VRONode> node = self.node.lock();
    if (!node) {
        NSLog(@"Aborted starting new animation, no target node specified");
        return;
    }
    
    __weak VRTManagedAnimation *weakSelf = self;
    _executableAnimation->execute(node,
                                  [weakSelf] {
                                      if (weakSelf) {
                                          [weakSelf onAnimationFinish];
                                      }
                                  });
    if (self.onStart) {
        self.onStart(nil);
    }
    self.state = VRTAnimationState::Running;
}

-(void)onAnimationFinish {
    if (self.onFinish) {
        self.onFinish(nil);
    }
    
    self.state = VRTAnimationState::Terminated;
    self.executableAnimation.reset();
    
    if (self.loop && self.run) {
        [self playAnimation];
    }
}

@end
