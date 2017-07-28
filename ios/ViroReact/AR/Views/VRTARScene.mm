//
//  VRTARScene.mm
//  ViroReact
//
//  Created by Andy Chu on 6/13/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <ViroKit/ViroKit.h>
#import "VRTARScene.h"
#import "VRTARNode.h"
#import "VRTARPlane.h"

@implementation VRTARScene {
    std::shared_ptr<VROARScene> _vroArScene;
    std::shared_ptr<VROARSceneDelegateiOS> _sceneDelegate;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _sceneDelegate = std::make_shared<VROARSceneDelegateiOS>(self);
        _vroArScene->setDelegate(_sceneDelegate);
    }
    return self;
}

- (void)initSceneController {
    // Create VROARSceneController.
    self.sceneController = std::make_shared<VROARSceneController>();
    
    // Create and attach delegate
    self.delegate = std::make_shared<VROSceneControllerDelegateiOS>(self);
    self.sceneController->setDelegate(self.delegate);
    
    //Set root node for this scene
    self.sceneController->getScene()->getRootNode()->addChildNode(self.node);
    
    _vroArScene = std::dynamic_pointer_cast<VROARScene>(self.sceneController->getScene());
}

#pragma mark VROARSceneDelegateProtocol Implementation

- (void)onTrackingInitialized {
    if (self.onTrackingInitializedViro) {
        self.onTrackingInitializedViro(@{});
    }
}

@end
