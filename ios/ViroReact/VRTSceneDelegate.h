//
//  VROSceneDelegate.h
//  React
//
//  Created by Vik Advani on 4/4/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ViroKit/ViroKit.h>

@protocol VRTSceneDelegate<NSObject>
- (void)sceneWillAppear:(VRORenderContext *)context driver:(VRODriver *)driver;
- (void)sceneDidAppear:(VRORenderContext *)context driver:(VRODriver *)driver;
- (void)sceneWillDisappear:(VRORenderContext *)context driver:(VRODriver *)driver;
- (void)sceneDidDisappear:(VRORenderContext *)context driver:(VRODriver *)driver;

- (void)startIncomingTransition:(VRORenderContext *)context duration:(float)duration;
- (void)startOutgoingTransition:(VRORenderContext *)context duration:(float)duration;
- (void)endIncomingTransition:(VRORenderContext *)context;
- (void)endOutgoingTransition:(VRORenderContext *)context;
- (void)animateIncomingTransition:(VRORenderContext *)context percentComplete:(float)t;
- (void)animateOutgoingTransition:(VRORenderContext *)context percentComplete:(float)t;

- (void)sceneWillRender:(const VRORenderContext *)context;

- (BOOL)isHoverable:(std::shared_ptr<VRONode>)node;
- (void)hoverOnNode:(std::shared_ptr<VRONode>)node;
- (void)hoverOffNode:(std::shared_ptr<VRONode>)node;

- (void)reticleTapped:(VROVector3f)ray context:(const VRORenderContext *)context;
@end
