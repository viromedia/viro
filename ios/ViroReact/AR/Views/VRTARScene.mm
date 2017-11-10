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
#import "VRTARHitTestUtil.h"
#import "VRTUtils.h"
#import "VRTImageAsyncLoader.h"

static NSString *const kVRTAmbientLightInfoKey = @"ambientLightInfo";
static NSString *const kVRTIntensityKey = @"intensity";
static NSString *const kVRTColorTemperatureKey = @"colorTemperature";

static NSString *const kCameraHitTestResults = @"hitTestResults";
static NSString *const kCameraOrientation = @"cameraOrientation";

@implementation VRTARScene {
    std::shared_ptr<VROARScene> _vroArScene;
    std::shared_ptr<VROARSceneDelegateiOS> _sceneDelegate;

    VRTImageAsyncLoader *_loader;
    std::shared_ptr<VROTexture> _pointCloudSurfaceTexture;
    std::shared_ptr<VROSurface> _pointCloudParticleSurface;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge]; // Invokes initSceneController
    if (self) {
        _loader = [[VRTImageAsyncLoader alloc] initWithDelegate:self];
        _sceneDelegate = std::make_shared<VROARSceneDelegateiOS>(self);
        _vroArScene->initDeclarativeSession();
        _vroArScene->setDelegate(_sceneDelegate);
        _vroArScene->getDeclarativeSession()->setDelegate(_sceneDelegate);
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
    _vroArScene->initDeclarativeSession();
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

- (void)setCanCameraARHitTest:(BOOL)canCameraARHitTest {
    _canCameraARHitTest = canCameraARHitTest;
    self.eventDelegate->setEnabledEvent(VROEventDelegate::EventAction::OnCameraARHitTest, canCameraARHitTest);
}

- (void)setDisplayPointCloud:(BOOL)displayPointCloud {
    _displayPointCloud = displayPointCloud;
    _vroArScene->displayPointCloud(displayPointCloud);
}

- (void)setPointCloudImage:(RCTImageSource *)pointCloudImage {
    _pointCloudImage = pointCloudImage;
    if (_pointCloudImage) {
        [_loader loadImage:_pointCloudImage];
    } else {
        _vroArScene->resetPointCloudSurface();
    }
}

- (void)setPointCloudScale:(NSArray<NSNumber *> *)pointCloudScale {
    if (!pointCloudScale) {
        _pointCloudScale = @[@.01f, @.01f, @.01f];
    } else {
        _pointCloudScale = [pointCloudScale copy];
    }
    float scaleValues[3];
    populateFloatArrayFromNSArray(_pointCloudScale, scaleValues, 3);
    _vroArScene->setPointCloudSurfaceScale({scaleValues[0], scaleValues[1], scaleValues[2]});
}

- (void)setPointCloudMaxPoints:(NSNumber *)pointCloudMaxPoints {
    if (!pointCloudMaxPoints) {
        _pointCloudMaxPoints = [NSNumber numberWithInt:500];
    } else {
        _pointCloudMaxPoints = pointCloudMaxPoints;
    }
    _vroArScene->setPointCloudMaxPoints([_pointCloudMaxPoints intValue]);
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

- (void)onCameraARHitTest:(int)source results:(std::vector<VROARHitTestResult>)results {
    if(self.onCameraARHitTestViro) {
        NSMutableArray *resultArray = [[NSMutableArray alloc] initWithCapacity:results.size()];
        for (VROARHitTestResult result : results) {
            [resultArray addObject:[VRTARHitTestUtil dictForARHitResult:result]];
        }
        
        NSArray<NSNumber *> * camOrientation = [self cameraOrientation];
    self.onCameraARHitTestViro(@{kCameraHitTestResults:resultArray,kCameraOrientation:camOrientation});
    }
}

#pragma mark VRTImageAsyncLoaderEventDelegate
- (void)imageLoaderDidStart:(VRTImageAsyncLoader *)loader {
    // no-op
}

- (void)imageLoaderDidEnd:(VRTImageAsyncLoader *)loader success:(BOOL)success image:(UIImage *)image {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (success) {
            _pointCloudSurfaceTexture = std::make_shared<VROTexture>(VROTextureInternalFormat::RGBA8, true,
                                                            VROMipmapMode::Runtime,
                                                            std::make_shared<VROImageiOS>(image, VROTextureInternalFormat::RGBA8),
                                                            VROStereoMode::None);
            if (!_pointCloudParticleSurface) {
                _pointCloudParticleSurface = VROSurface::createSurface(1, 1);
            }
            
            _pointCloudParticleSurface->getMaterials()[0]->getDiffuse().setTexture(_pointCloudSurfaceTexture);

            if (_vroArScene) {
                _vroArScene->setPointCloudSurface(_pointCloudParticleSurface);
            }
        } else {
            perror("Viro: Error loading point cloud image resource");
        }
    });
}

@end
