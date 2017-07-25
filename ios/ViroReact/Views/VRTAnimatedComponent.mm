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
#import "VRTManagedAnimation.h"
#import "VRTNode.h"

@interface VRTAnimatedComponent ()

@property (readwrite, nonatomic) BOOL viewAdded;
@property (readwrite, nonatomic) VRTAnimationManager *animationManager;
@property (readwrite, nonatomic) VRTManagedAnimation *managedAnimation;

@end

@implementation VRTAnimatedComponent {
    VRTAnimationManager *_animationManager;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge  {
    self = [super initWithBridge:bridge];
    if (self) {
        self.managedAnimation = [[VRTManagedAnimation alloc] init];
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
        [self.managedAnimation updateAnimation];
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
    [self.managedAnimation updateAnimation];
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

- (void)setOnStartViro:(RCTDirectEventBlock)onStartViro {
    self.managedAnimation.onStart = onStartViro;
}

- (void)setOnFinishViro:(RCTDirectEventBlock)onFinishViro {
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
    
    // Re-retrieve the animation from the animation manager each time, in case
    // it changed. This is a cheap dictionary lookup.
    self.managedAnimation.animation = [_animationManager animationForName:self.animation]->copy();
    [self.managedAnimation updateAnimation];
}

@end
