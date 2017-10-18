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
#import "VRTARUtils.h"
#import "VRTARSceneNavigator.h"

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
    
    _vroArScene = std::dynamic_pointer_cast<VROARScene>(self.sceneController->getScene());
    self.portalTraversalListener = std::make_shared<VROPortalTraversalListener>(_vroArScene);
}

- (void)setAnchorDetectionTypes:(NSArray<NSString *> *)types {
    _anchorDetectionTypes = types;
    UIView *parent = [self superview];
    if ([parent isKindOfClass:[VRTARSceneNavigator class]]) {
        UIView *rootView = [((VRTARSceneNavigator *)parent) rootVROView];
        if ([rootView isKindOfClass:[VROViewAR class]]) {
            std::shared_ptr<VROARSession> session = [((VROViewAR *)rootView) getARSession];
            std::set<VROAnchorDetection> detectionTypes;
            for (NSString *type in types) {
                if ([type caseInsensitiveCompare:@"None"] == NSOrderedSame) {
                    detectionTypes.insert(VROAnchorDetection::None);
                } else if([type caseInsensitiveCompare:@"PlanesHorizontal"] == NSOrderedSame) {
                    detectionTypes.insert(VROAnchorDetection::PlanesHorizontal);
                }
            }
            session->setAnchorDetection(detectionTypes);
        }
    }
}

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

- (void)onAnchorFound:(std::shared_ptr<VROARAnchor>)anchor {
    if (self.onAnchorFoundViro) {
        self.onAnchorFoundViro(@{@"anchor" : [VRTARUtils createDictionaryFromAnchor:anchor]});
    }
}

- (void)onAnchorUpdated:(std::shared_ptr<VROARAnchor>)anchor {
    if (self.onAnchorUpdatedViro) {
        self.onAnchorUpdatedViro(@{@"anchor" : [VRTARUtils createDictionaryFromAnchor:anchor]});
    }
}

- (void)onAnchorRemoved:(std::shared_ptr<VROARAnchor>)anchor {
    if (self.onAnchorRemovedViro) {
        self.onAnchorRemovedViro(@{@"anchor" : [VRTARUtils createDictionaryFromAnchor:anchor]});
    }
}

@end
