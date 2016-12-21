//
//  VROSceneController.h
//  ViroRenderer
//
//  Created by Raj Advani on 3/25/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <memory>
#import "VROVector3f.h"
#import "VRORenderDelegate.h"
#import "VROView.h"

class VRONode;
class VROScene;
class VRORenderContext;
class VROHoverDelegate;
class VROSceneControlleriOS;

@class VROScreenUIView;

@interface VROSceneController : NSObject

- (id)init;

@property (readonly, nonatomic) std::shared_ptr<VROSceneControlleriOS> internal;

#pragma mark - Control Methods

- (void)setHoverEnabled:(BOOL)enabled boundsOnly:(BOOL)boundsOnly;
- (std::shared_ptr<VROScene>)scene;

#pragma mark - Delegate Methods

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
