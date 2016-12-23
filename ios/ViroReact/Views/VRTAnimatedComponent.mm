//
//  VRTAnimatedComponent.m
//  React
//
//  Created by Vik Advani on 2/19/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTAnimatedComponent.h"
#import "VRTAnimationManager.h"
#import "VRTNode.h"

@implementation VRTAnimatedComponent {
    /*
     Dictionary used for animated values that are additive, ie rotationX += 15 per loop.
     Store current rotation values in this dictionary.
     */
    NSMutableDictionary *_propCheckPoints;
    std::shared_ptr<VROGeometry> _childGeometry;
    BOOL _viewAdded;
    VRTAnimationManager *_animationManager;
    NSInteger _completedAnimationChainCount;
    BOOL _scheduledNewAnimation;
    std::vector<std::shared_ptr<VROTransaction>> _currentRunningAnimationTransactions;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge  {
    self = [super initWithBridge:bridge];
    if (self) {
        _propCheckPoints = [[NSMutableDictionary alloc] init];

        self.delay = -1.0f;
        self.loop = false;
        self.run = true;
        _animationManager = [self.bridge animationManager];
        _completedAnimationChainCount = 0;
        _viewAdded = false;
        _scheduledNewAnimation = false;
    }
    return self;
}

#pragma mark - VRTView overrides.

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

    // If the parent view is added before the child, viewWillAppear:
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

- (void)viewWillAppear {
    [super viewWillAppear];
    _viewAdded = true;

    [self updateAnimation];
}

- (void)setRun:(BOOL)run {
    _run = run;
    [self updateAnimation];
}

- (void)setAnimation:(NSString *)animation {
  _animation = animation;
  if (!_currentRunningAnimationTransactions.empty()) {
    _currentRunningAnimationTransactions.clear();
  }

  [self updateAnimation];
}

// Override node to return vroLayer of vroUIView
- (std::shared_ptr<VRONode>) node {
    return [self.vroSubview node];
}

- (id<VROComponent>)reactSuperview {
    return nil;
}

- (NSNumber *)reactTagAtPoint:(CGPoint)point {
    return nil;
}

- (void)updateAnimation {
   if (self.vroSubview == nil) {
     return;
   }
  
   _childGeometry = [self.vroSubview node]->getGeometry();
   if (self.run){
      [self startAnimation];
   } else {
      [self pauseAnimation];
  }
}

/**
 * Schedules the start of a new animation. If there are currently running
 * animations, it resumes them. If there is a delay associated with this
 * AnimationComponent, we start a new animation with the given delay.
 */
-(void)startAnimation{
    if (!_currentRunningAnimationTransactions.empty()){
        std::vector<std::shared_ptr<VROTransaction>>::iterator it;
        std::vector<std::shared_ptr<VROTransaction>> runningTransactions = _currentRunningAnimationTransactions;
        for (it = runningTransactions.begin(); it != runningTransactions.end(); ++it) {
            std::shared_ptr<VROTransaction> transaction = *it;
            VROTransaction::resume(transaction);
        }
    } else {
        _scheduledNewAnimation = true;
        [self performSelector:@selector(startNewAnimation) withObject:self afterDelay: (self.delay / 1000)];
    }
}

/**
 * Pauses currently running animations.
 */
-(void)pauseAnimation{
    if (!_currentRunningAnimationTransactions.empty()){
        std::vector<std::shared_ptr<VROTransaction>>::iterator it;
        std::vector<std::shared_ptr<VROTransaction>> runningTransactions = _currentRunningAnimationTransactions;
        for (it = runningTransactions.begin(); it != runningTransactions.end(); ++it) {
            std::shared_ptr<VROTransaction> transaction = *it;
            VROTransaction::pause(transaction);
        }
    } else if (_scheduledNewAnimation){
        _scheduledNewAnimation = false;
        [NSObject cancelPreviousPerformRequestsWithTarget: self selector:@selector(startNewAnimation) object:self];
    }
}

/**
 * Grabs an Animation Property corresponding to the animation name associated
 * with this AnimatedComponent from the animation manager and animates them
 * (either as a single animation or as a part of an array of
 * chain of animations).
 */
-(void)startNewAnimation {
    _scheduledNewAnimation = false;
    if(_childGeometry != nil){
        // Show the geometry if it isn't being shown yet.
        [self.vroSubview node]->setGeometry(_childGeometry);
    }

    // If we have a bunch of animation chains, start animating them in parallel.
    NSArray *chainArray = [_animationManager getAnimationChainForName:self.animation];
    if (chainArray != nil && [chainArray count] >0){
        _completedAnimationChainCount = 0;
        for (NSArray *chain in chainArray){
            [self animatePropertyChain:chain atIndex:0];
        }
        return;
    }

    // Else we are animating a single animation block.
    AnimationProperty *animProperty = [_animationManager getAnimationPropertyForName:self.animation];
    if (animProperty) {
        [self animateProperty:animProperty];
        return;
    }
    NSLog(@"ERROR: Attempted to start processing unknown animation!");
}

- (void)animateProperty:(AnimationProperty *)animProperty{
    __weak VRTAnimatedComponent *weakSelf = self;
    AnimationFinishedBlock animationFinishedCallback = ^(NSArray *currentChain, NSInteger currentIndex){
        if (weakSelf) {
            [weakSelf onAnimationFinish];
        }
    };
    [self transactAnimationProperties:animProperty
                         withCallback:animationFinishedCallback
                            withChain:nil
                       withChainIndex:0
                         withpropChck:_propCheckPoints];
}

- (void)animatePropertyChain:(NSArray *)animationChain atIndex:(NSInteger)index{
    __weak VRTAnimatedComponent *weakSelf = self;
    AnimationFinishedBlock animationFinishedCallback = ^(NSArray *chain, NSInteger currentIndex){
        if (!weakSelf) {
            return;
        }

        [weakSelf onAnimatePropertyChainCallback:chain atIndex:currentIndex];
    };

    // Begin the animation with the above animationFinishedCallback.
    NSString *currentAnimation = [animationChain objectAtIndex:index];
    AnimationProperty *animProperty = [_animationManager getAnimationPropertyForName:currentAnimation];
    [self transactAnimationProperties:animProperty
                         withCallback:animationFinishedCallback
                            withChain:animationChain
                       withChainIndex:index
                         withpropChck:_propCheckPoints];
}

- (void)onAnimatePropertyChainCallback:(NSArray*)chain atIndex:(NSInteger)currentIndex{
    // Animate the rest of the animations within this chain if we haven't yet done so.
    if (currentIndex < [chain count] -1){
        [self animatePropertyChain:chain atIndex:currentIndex+1];
        return;
    }

    // Else, we have completed this animation chain. Finish the animation if we
    // have completed animating all animation chains.
    NSArray *chainArray = [_animationManager getAnimationChainForName:self.animation];
    _completedAnimationChainCount++;
    if (_completedAnimationChainCount >= [chainArray count]){
        [self onAnimationFinish];
    }
}

- (void)transactAnimationProperties:(AnimationProperty *)animationProperty
                       withCallback:(AnimationFinishedBlock)animationFinishedBlock
                          withChain:(NSArray*)chain
                     withChainIndex:(NSInteger) chainIndex
                       withpropChck:(NSMutableDictionary *)propCheckPoints{
    VROTransaction::begin();
    VROTransaction::setAnimationDelay(animationProperty.delayMilliseconds);
    VROTransaction::setAnimationDuration(animationProperty.durationMilliseconds);
    VROTransaction::setTimingFunction(animationProperty.functionType);
    VROTransaction::setFinishCallback([animationFinishedBlock, chain, chainIndex]() -> void {
        animationFinishedBlock(chain, chainIndex);
    });

    [_animationManager mapAnimatedPropertiesToNode:[self.vroSubview node].get()
                                        properties:animationProperty.propertyDictionary
                                   propCheckPoints:propCheckPoints];
    _currentRunningAnimationTransactions.push_back(VROTransaction::commit());
}

-(void)dealloc {
    // Cleanup any transactions that were paused.
    if (!_currentRunningAnimationTransactions.empty()){
        std::vector<std::shared_ptr<VROTransaction>>::iterator it;
        std::vector<std::shared_ptr<VROTransaction>> runningTransactions = _currentRunningAnimationTransactions;
        for (it = runningTransactions.begin(); it != runningTransactions.end(); ++it) {
            std::shared_ptr<VROTransaction> transaction = *it;
            VROTransaction::terminate(transaction);
        }
    }
}

-(void)onAnimationFinish {
    _currentRunningAnimationTransactions.clear();
    if (self.onFinish){
        self.onFinish(nil);
    }

    if (self.loop && self.run){
        [self startAnimation];
    }
}
@end
