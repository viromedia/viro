//
//  VRTARScene.mm
//  ViroReact
//
//  Created by Andy Chu on 6/13/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <ViroKit/ViroKit.h>
#import "VRTARScene.h"
#import "VRTARAnchorNode.h"
#import "VRTARPlane.h"

static NSString *const kVRTAmbientLightInfoKey = @"ambientLightInfo";
static NSString *const kVRTIntensityKey = @"intensity";
static NSString *const kVRTColorTemperatureKey = @"colorTemperature";

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
    self.portalTraversalListener = std::make_shared<VROPortalTraversalListener>(_vroArScene);
}

// Add portal traversal listener for this AR scene.


#pragma mark VROARSceneDelegateProtocol Implementation

- (void)onTrackingInitialized {
    if (self.onTrackingInitializedViro) {
        self.onTrackingInitializedViro(@{});
    }
}

- (void)onAmbientLightUpdate:(float)intensity colorTemperature:(float)colorTemperature {
    if (self.onAmbientLightUpdateViro) {
        self.onAmbientLightUpdateViro(@{kVRTAmbientLightInfoKey: @{ kVRTIntensityKey : @(intensity),
                                                                    kVRTColorTemperatureKey : @(colorTemperature)
                                                                    }});
    }
}

@end
