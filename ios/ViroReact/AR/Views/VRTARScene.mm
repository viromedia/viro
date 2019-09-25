//
//  VRTARScene.mm
//  ViroReact
//
//  Created by Andy Chu on 6/13/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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
static NSString *const kVRTAmbientIntensityKey = @"intensity";
static NSString *const kVRTAmbientColorKey = @"color";

static NSString *const kCameraHitTestResults = @"hitTestResults";
static NSString *const kCameraOrientation = @"cameraOrientation";

static NSString *const kVRTTrackingState = @"state";
static NSString *const kVRTTrackingStateReason = @"reason";

static NSString *const kPointCloudKey = @"pointCloud";

@implementation VRTARScene {
    std::shared_ptr<VROARScene> _vroArScene;
    std::shared_ptr<VROARSceneDelegateiOS> _sceneDelegate;

    VRTImageAsyncLoader *_loader;
    std::shared_ptr<VROTexture> _pointCloudSurfaceTexture;
    std::shared_ptr<VROSurface> _pointCloudParticleSurface;
    std::set<VROAnchorDetection> _nativeDetectionTypes;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge]; // Invokes initSceneController
    if (self) {
        _loader = [[VRTImageAsyncLoader alloc] initWithDelegate:self];
        _sceneDelegate = std::make_shared<VROARSceneDelegateiOS>(self);
        _vroArScene->initDeclarativeSession();
        _vroArScene->setDelegate(_sceneDelegate);
        _vroArScene->getDeclarativeSession()->setDelegate(_sceneDelegate);
        _nativeDetectionTypes = { VROAnchorDetection::PlanesHorizontal }; // default detection type is horizontal plane
        _vroArScene->setAnchorDetectionTypes(_nativeDetectionTypes);
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
    
    std::set<VROAnchorDetection> detectionTypes;
    for (NSString *type in _anchorDetectionTypes) {
        if ([type caseInsensitiveCompare:@"planesHorizontal"] == NSOrderedSame) {
            detectionTypes.insert(VROAnchorDetection::PlanesHorizontal);
        } else if ([type caseInsensitiveCompare:@"planesVertical"] == NSOrderedSame) {
            detectionTypes.insert(VROAnchorDetection::PlanesVertical);
        }
    }
    
    _nativeDetectionTypes = detectionTypes;

    if (_vroArScene) {
        _vroArScene->setAnchorDetectionTypes(detectionTypes);
    }
}

#pragma mark VROARSceneDelegateProtocol Implementation

- (void)onTrackingUpdated:(VROARTrackingState)state withReason:(VROARTrackingStateReason)reason {
    if (self.onTrackingUpdatedViro) {
        self.onTrackingUpdatedViro(@{kVRTTrackingState : @((int)state),
                                     kVRTTrackingStateReason : @((int)reason)});
    }
}

- (void)onAmbientLightUpdate:(float)intensity color:(VROVector3f)color {
    if (self.onAmbientLightUpdateViro) {
        NSString *lightColor = [NSString stringWithFormat:@"#%02x%02x%02x",
                                (int) MIN(255, MAX(0, color.x * 255)),
                                (int) MIN(255, MAX(0, color.y * 255)),
                                (int) MIN(255, MAX(0, color.z * 255))];
        self.onAmbientLightUpdateViro(@{kVRTAmbientLightInfoKey: @{ kVRTAmbientIntensityKey : @(intensity),
                                                                    kVRTAmbientColorKey : lightColor
                                                                    }});
    }
}

- (void)setCanCameraARHitTest:(BOOL)canCameraARHitTest {
    _canCameraARHitTest = canCameraARHitTest;
    self.eventDelegate->setEnabledEvent(VROEventDelegate::EventAction::OnCameraARHitTest, canCameraARHitTest);
}

- (void)setCanARPointCloudUpdate:(BOOL)canARPointCloudUpdate {
    _canARPointCloudUpdate = canARPointCloudUpdate;
    self.eventDelegate->setEnabledEvent(VROEventDelegate::EventAction::OnARPointCloudUpdate, _canARPointCloudUpdate);
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

- (void)onCameraARHitTest:(std::vector<std::shared_ptr<VROARHitTestResult>>)results {
    if(self.onCameraARHitTestViro) {
        NSMutableArray *resultArray = [[NSMutableArray alloc] initWithCapacity:results.size()];
        for (std::shared_ptr<VROARHitTestResult> &result : results) {
            [resultArray addObject:[VRTARHitTestUtil dictForARHitResult:result]];
        }
        
        NSArray<NSNumber *> * camOrientation = [self cameraOrientation];
        self.onCameraARHitTestViro(@{kCameraHitTestResults:resultArray,kCameraOrientation:camOrientation});
    }
}

- (void)onARPointCloudUpdate:(std::shared_ptr<VROARPointCloud>)pointCloud {
    if (self.onARPointCloudUpdateViro) {
        self.onARPointCloudUpdateViro(@{kPointCloudKey:[VRTARUtils createDictionaryFromARPointCloud:pointCloud]});
    }
}

#pragma mark VRTImageAsyncLoaderEventDelegate
- (void)imageLoaderDidStart:(VRTImageAsyncLoader *)loader {
    // no-op
}

- (void)imageLoaderDidEnd:(VRTImageAsyncLoader *)loader success:(BOOL)success image:(UIImage *)image {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (success) {
            _pointCloudSurfaceTexture = std::make_shared<VROTexture>(true,
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
