//
//  VRTAnimatedComponent.m
//  React
//
//  Created by Vik Advani on 2/19/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#include <React/RCTComponent.h>
#import <React/RCTUIManager.h>
#import "VRTAnimatedComponent.h"
#import "VRTAnimationManager.h"
#import "VRTNode.h"

enum class VRTAnimatedComponentState {
    Scheduled,
    Running,
    Paused,
    Terminated
};

@interface VRTAnimatedComponent ()

@property (readwrite, nonatomic) BOOL viewAdded;
@property (readwrite, nonatomic) VRTAnimatedComponentState state;
@property (readwrite, nonatomic) VRTAnimationManager *animationManager;
@property (readwrite, nonatomic) std::shared_ptr<VROExecutableAnimation> executableAnimation;
@property (readwrite, nonatomic) BOOL animationNeedsUpdate;

@end

@implementation VRTAnimatedComponent {

    VRTAnimationManager *_animationManager;
    
}

- (instancetype)initWithBridge:(RCTBridge *)bridge  {
    self = [super initWithBridge:bridge];
    if (self) {
        self.delay = -1.0f;
        self.loop = false;
        self.run = true;
        self.animationNeedsUpdate = false;
      
        self.viewAdded = false;
        self.state = VRTAnimatedComponentState::Terminated;
        
        _animationManager = [self.bridge animationManager];
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

#pragma mark - VRTView overrides

- (std::shared_ptr<VRONode>) node {
    return [self.vroSubview node];
}

- (id<RCTComponent>)reactSuperview {
    return nil;
}

- (NSNumber *)reactTagAtPoint:(CGPoint)point {
    return nil;
}

- (void)insertReactSubview:(UIView *)subview atIndex:(NSInteger)atIndex {
    self.vroSubview = (VRTNode *)subview;

    BOOL childFound = false;
    if (self.superview != nil) {
        // The supernode of a VRTAnimatedComponent is always a VRTNode
        VRTNode *supernodeView = (VRTNode *) self.superview;

        std::vector<std::shared_ptr<VRONode>> subnodeArray = supernodeView.node->getSubnodes();
        for (std::shared_ptr<VRONode> node: subnodeArray){
            if (node.get() == self.vroSubview.node.get()) {
                childFound = true;
                break;
            }
        }
        if (!childFound) {
            supernodeView.node->addChildNode(self.vroSubview.node);
        }
    }

    // If the parent view is added before the child, sceneWillAppear:
    // will have already been invoked, so run the animation here
    if (_viewAdded){
        [self updateAnimation];
    }

    [super insertReactSubview:subview atIndex:atIndex];
}

- (void)removeReactSubview:(UIView *)subview {
    _viewAdded = false;

    self.vroSubview.node->removeFromParentNode();
    [super removeReactSubview:subview];
}

- (void)sceneWillAppear {
    [super sceneWillAppear];
    _viewAdded = true;
    [self updateAnimation];
}

- (void)setRun:(BOOL)run {
    _run = run;
    _animationNeedsUpdate = YES;
}

- (void)setLoop:(BOOL)loop {
    _loop = loop;
    _animationNeedsUpdate = YES;
}

- (void)setAnimation:(NSString *)animation {
    if (self.executableAnimation) {
        self.executableAnimation->terminate();
    }
    self.state = VRTAnimatedComponentState::Terminated;
    
    _animation = animation;
    _animationNeedsUpdate = YES;
}

- (void)updateAnimation {
    if (!self.vroSubview) {
        return;
    }
    
    if (self.run) {
        [self playAnimation];
    }
    else {
        [self pauseAnimation];
    }
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    if (_animationNeedsUpdate) {
        [self updateAnimation];
        _animationNeedsUpdate = NO;
    }
}

/*
 Plays the animation. If the animation is paused, resumes the animation.
 Otherwise starts a new animation if is not running. If there is a delay 
 associated with this VRTAnimatedComponent, we start a new animation with the 
 given delay.
 */
- (void)playAnimation {
    if (self.state == VRTAnimatedComponentState::Paused) {
        self.executableAnimation->resume();
        self.state = VRTAnimatedComponentState::Running;
    }
    else if (self.state == VRTAnimatedComponentState::Terminated) {
        self.state = VRTAnimatedComponentState::Scheduled;
        [self performSelector:@selector(startAnimation) withObject:self afterDelay:self.delay / 1000.0];
    }
    else {
        NSLog(@"Unable to play animation in state %d", self.state);
    }
}

- (void)pauseAnimation {
    if (self.state == VRTAnimatedComponentState::Running) {
        self.executableAnimation->pause();
        self.state = VRTAnimatedComponentState::Paused;
    }
    else if (self.state == VRTAnimatedComponentState::Scheduled) {
        [NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(startAnimation) object:self];
        self.state = VRTAnimatedComponentState::Terminated;
        self.executableAnimation.reset();
    }
}

- (void)startAnimation {
    if (self.state != VRTAnimatedComponentState::Scheduled) {
        NSLog(@"Aborted starting new animation, was no longer scheduled");
        return;
    }
    if ([_animationManager animationForName:self.animation] == nullptr) {
        RCTLogError(@"Unable to find Animation with name %@", self.animation);
        return;
    }

    _executableAnimation = [_animationManager animationForName:self.animation]->copy();

    if (_executableAnimation) {
        __weak VRTAnimatedComponent *weakSelf = self;
        _executableAnimation->execute(self.vroSubview.node,
                                      [weakSelf] {
                                          if (weakSelf) {
                                              [weakSelf onAnimationFinish];
                                          }
                                      });
        if (self.onStartViro) {
            self.onStartViro(nil);
        }
        self.state = VRTAnimatedComponentState::Running;
    }
    else {
        NSLog(@"Error: attempted to start processing unknown animation!");
        self.state = VRTAnimatedComponentState::Terminated;
    }
}

-(void)onAnimationFinish {
    if (self.onFinishViro) {
        self.onFinishViro(nil);
    }
    
    self.state = VRTAnimatedComponentState::Terminated;
    self.executableAnimation.reset();
    
    if (self.loop && self.run) {
        [self playAnimation];
    }
}

@end
