//
//  VROSceneController.m
//  React
//
//  Created by Vik Advani on 4/4/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "RCTSceneController.h"

@implementation RCTSceneController

- (instancetype)initWithView:(id <VROView>)view sceneDelegate:(id<VRTSceneDelegate>)delegate {
  self = [super init];
  if(self){
    self.sceneDelegate = delegate;
  }
  return self;
}

- (void)sceneWillAppear:(VRORenderContext *)context driver:(VRODriver *)driver {
  [super sceneDidAppear:context driver:driver];
  [self.sceneDelegate sceneWillAppear:context driver:driver];
}
- (void)sceneDidAppear:(VRORenderContext *)context driver:(VRODriver *)driver {
  [super sceneDidAppear:context driver:driver];
  [self.sceneDelegate sceneDidAppear:context driver:driver];
}
- (void)sceneWillDisappear:(VRORenderContext *)context driver:(VRODriver *)driver {
  [super sceneWillDisappear:context driver:driver];
  [self.sceneDelegate sceneWillDisappear:context driver:driver];
}

- (void)sceneDidDisappear:(VRORenderContext *)context driver:(VRODriver *)driver {
  [super sceneDidDisappear:context driver:driver];
  [self.sceneDelegate sceneDidDisappear:context driver:driver];
}

- (void)startIncomingTransition:(VRORenderContext *)context duration:(float)duration {
  //We don't call super here since that simply invokes default animation that doesn't apply in our case.
  [self.sceneDelegate startIncomingTransition:context duration:duration];
}

- (void)startOutgoingTransition:(VRORenderContext *)context duration:(float)duration {
  //We don't call super here since that simply invokes default animation that doesn't apply in our case.
  [self.sceneDelegate startOutgoingTransition:context duration:duration];
}
- (void)endIncomingTransition:(VRORenderContext *)context {
  [super endIncomingTransition:context];
  [self.sceneDelegate endIncomingTransition:context];
}

- (void)endOutgoingTransition:(VRORenderContext *)context {
  [super endOutgoingTransition:context];
  [self.sceneDelegate endOutgoingTransition:context];
}

- (void)animateIncomingTransition:(VRORenderContext *)context percentComplete:(float)t {
   [super animateIncomingTransition:context percentComplete:t];
   [self.sceneDelegate animateIncomingTransition:context percentComplete:t];
}

- (void)animateOutgoingTransition:(VRORenderContext *)context percentComplete:(float)t {
  [super animateOutgoingTransition:context percentComplete:t];
  [self.sceneDelegate animateOutgoingTransition:context percentComplete:t];
}

- (void)sceneWillRender:(const VRORenderContext *)context {
  [super sceneWillRender:context];
  [self.sceneDelegate sceneWillRender:context];
}

- (void)reticleTapped:(VROVector3f)ray context:(const VRORenderContext *)context {
  [super reticleTapped:ray context:context];
  [self.sceneDelegate reticleTapped:ray context:context];
}

- (BOOL)isHoverable:(std::shared_ptr<VRONode>)node {
  return [self.sceneDelegate isHoverable:node];
}

- (void)hoverOnNode:(std::shared_ptr<VRONode>)node {
  [self.sceneDelegate hoverOnNode:node];
}

- (void)hoverOffNode:(std::shared_ptr<VRONode>)node {
  [self.sceneDelegate hoverOffNode:node];
}

@end
