//
//  VRTAnimatedComponent.m
//  React
//
//  Created by Vik Advani on 2/19/16.
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

#include <React/RCTComponent.h>
#import <React/RCTUIManager.h>
#import "VRTAnimatedComponent.h"
#import "VRTAnimationManager.h"
#import "VRTManagedAnimation.h"
#import "VRTNode.h"

@interface VRTAnimatedComponentAnimation : VRTManagedAnimation

@property (readwrite, nonatomic) NSString *animationName;
@property (readwrite, nonatomic) VRTAnimationManager *animationManager;

- (std::shared_ptr<VROExecutableAnimation>)loadAnimation;

@end

@implementation VRTAnimatedComponentAnimation

- (std::shared_ptr<VROExecutableAnimation>)loadAnimation {
    if (self.animationName != nil) {
        return [self.animationManager animationForName:self.animationName]->copy();
    }
    else {
        return nullptr;
    }
}

- (void)setAnimationName:(NSString *)animationName {
    _animationName = [animationName copy];
    [super updateAnimation];
}

@end

@interface VRTAnimatedComponent ()

@property (readwrite, nonatomic) BOOL viewAdded;
@property (readwrite, nonatomic) VRTAnimationManager *animationManager;
@property (readwrite, nonatomic) VRTAnimatedComponentAnimation *managedAnimation;

@end

@implementation VRTAnimatedComponent {
    VRTAnimationManager *_animationManager;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge  {
    self = [super initWithBridge:bridge];
    if (self) {
        self.managedAnimation = [[VRTAnimatedComponentAnimation alloc] init];
        self.managedAnimation.animationManager = [self.bridge animationManager];
        self.animationManager = [self.bridge animationManager];
        self.viewAdded = false;
    }
    return self;
}

- (void)dealloc {
    
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
    self.managedAnimation.node = self.vroSubview.node;
    
    BOOL childFound = false;
    if (self.superview != nil) {
        // The supernode of a VRTAnimatedComponent is always a VRTNode
        VRTNode *supernodeView = (VRTNode *) self.superview;
        
        std::vector<std::shared_ptr<VRONode>> subnodeArray = supernodeView.node->getChildNodes();
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
    // (Note setting the animation name will trigger updateAnimation.
    if (_viewAdded) {
        if (self.animation) {
            self.managedAnimation.animationName = self.animation;
        }
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
    
    if (self.animation) {
        self.managedAnimation.animationName = self.animation;
    }
}

#pragma mark - Animation Properties

- (void)setRun:(BOOL)run {
    self.managedAnimation.run = run;
}

- (void)setLoop:(BOOL)loop {
    self.managedAnimation.loop = loop;
}

- (void)setDelay:(float)delay {
    self.managedAnimation.delay = delay;
}

- (void)setOnAnimationStartViro:(RCTDirectEventBlock)onStartViro {
    self.managedAnimation.onStart = onStartViro;
}

- (void)setOnAnimationFinishViro:(RCTDirectEventBlock)onFinishViro {
    self.managedAnimation.onFinish = onFinishViro;
}

- (void)setAnimation:(NSString *)animation {
    if ([_animationManager animationForName:animation] == nullptr) {
        RCTLogError(@"Unable to find Animation with name %@", self.animation);
        return;
    }
    
    _animation = animation;
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    if (!self.vroSubview) {
        return;
    }
    self.managedAnimation.animationName = self.animation;
}

@end
