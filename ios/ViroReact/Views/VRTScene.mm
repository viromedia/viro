//
//  VRTScene.m
//  React
//
//  Created by Vik Advani on 12/11/15.
//  Copyright © 2015 Viro Media. All rights reserved.
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
#import <React/RCTConvert.h>
#import "VRTScene.h"
#import "VRTCamera.h"
#import "VRTOrbitCamera.h"
#import "VRTMaterialManager.h"

#import "VRTNode.h"
#import "VRTLog.h"
#import "VRTTreeNode.h"
#import <map>

static NSString *const kPlatformInfoKey = @"platformInfoViro";
static NSString *const kVrPlatformKey = @"vrPlatform";
static NSString *const kPlatformKey = @"platform";
static NSString *const kHeadsetKey = @"headset";
static NSString *const kControllerKey = @"controller";


static NSString *const kSizeKey = @"size";
static NSString *const kWallMaterialKey = @"wallMaterial";
static NSString *const kCeilingMaterialKey = @"ceilingMaterial";
static NSString *const kFloorMaterialKey = @"floorMaterial";
static NSString *const kDefaultMaterial = @"transparent";
static NSString *const kCameraTransform = @"cameraTransform";

static NSArray<NSNumber *> *const kDefaultSize = @[@(0), @(0), @(0)];

@implementation VRTScene {
    id <VROView> _vroView;
    VRTCamera *_camera;
    std::shared_ptr<VROScene> _vroScene;
    NSArray<NSNumber *> *_size;
    NSString *_wallMaterial;
    NSString *_ceilingMaterial;
    NSString *_floorMaterial;
    
    /*
     This BOOL ensures we only set the sound room when necessary, because doing so
     activates GVR, and in current GVR versions this can cause issues with video
     recording, until VIRO-2944 is resolved.
     */
    BOOL _soundRoomSet;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge  {
    self = [super initWithBridge:bridge];
    if (self) {
        _recticleEnabled = true;
    }
    _size = kDefaultSize;
    _wallMaterial = kDefaultMaterial;
    _ceilingMaterial = kDefaultMaterial;
    _floorMaterial = kDefaultMaterial;
    _soundRoomSet = NO;
    
    return self;
}

- (void)initSceneController {
    // Create VROSceneController.
    _sceneController = std::make_shared<VROSceneController>();
    
    // Create and attach delegate
    _delegate = std::make_shared<VROSceneControllerDelegateiOS>(self);
    _sceneController->setDelegate(_delegate);
    
    _vroScene = std::dynamic_pointer_cast<VROScene>(self.sceneController->getScene());
}

- (std::shared_ptr<VRONode>)createVroNode {
    [self initSceneController];
    return self.sceneController->getScene()->getRootNode();
}

- (void)setView:(id <VROView>)view {
    _vroView = view;
    
    [self setCameraIfAvailable];
    [self notifyOnPlatformUpdate];
}


- (id<VROView>)getVROView {
    return _vroView;
}

-(void)setDriver:(std::shared_ptr<VRODriver>)driver {
    [super setDriver:driver];
    if (_soundRoomSet) {
        self.driver->setSoundRoom([[_size objectAtIndex:0] floatValue],
                                  [[_size objectAtIndex:1] floatValue],
                                  [[_size objectAtIndex:2] floatValue],
                                  [_wallMaterial UTF8String], [_ceilingMaterial UTF8String], [_floorMaterial UTF8String]);
    }
}

- (std::shared_ptr<VROSceneController>)sceneController {
    return _sceneController;
}

- (std::shared_ptr<VROScene>)scene {
    return _sceneController->getScene();
}

- (void)setSoundRoom:(NSDictionary *)soundRoom {
    if (soundRoom) {
        _soundRoomSet = YES;
        _size = [soundRoom objectForKey:kSizeKey] ? [soundRoom objectForKey:kSizeKey] : kDefaultSize;
        _wallMaterial = [soundRoom objectForKey:kWallMaterialKey] ? [soundRoom objectForKey:kWallMaterialKey] : kDefaultMaterial;
        _ceilingMaterial = [soundRoom objectForKey:kCeilingMaterialKey] ? [soundRoom objectForKey:kCeilingMaterialKey] : kCeilingMaterialKey;
        _floorMaterial = [soundRoom objectForKey:kFloorMaterialKey] ? [soundRoom objectForKey:kFloorMaterialKey] : kFloorMaterialKey;
    } else {
        _soundRoomSet = NO;
        _size = kDefaultSize;
        _wallMaterial = kDefaultMaterial;
        _ceilingMaterial = kDefaultMaterial;
        _floorMaterial = kDefaultMaterial;
    }
    
    if (self.driver) {
        self.driver->setSoundRoom([[_size objectAtIndex:0] floatValue], [[_size objectAtIndex:1] floatValue], [[_size objectAtIndex:2] floatValue], [_wallMaterial UTF8String], [_ceilingMaterial UTF8String], [_floorMaterial UTF8String]);
    }
}

- (void)setOnPlatformUpdateViro:(RCTDirectEventBlock)onPlatformUpdateViro {
    _onPlatformUpdateViro = onPlatformUpdateViro;
    [self notifyOnPlatformUpdate];
}

- (void)notifyOnPlatformUpdate {
    if (_onPlatformUpdateViro && _vroView) {
        _onPlatformUpdateViro(@{kPlatformInfoKey: @{kPlatformKey : [_vroView getPlatform],
                                                    kHeadsetKey : [_vroView getHeadset],
                                                    kControllerKey : [_vroView getController]}});
    }
}

#pragma mark - Camera

- (void)setCameraIfAvailable {
    if (!_camera || !_vroView) {
        return;
    }
    
    [_vroView setPointOfView:_camera.nodeRootTransformCamera];
    NSLog(@"Set camera with rotation type %d", _camera.nodeRootTransformCamera->getCamera()->getRotationType());
}

- (void)setCamera:(VRTCamera *)camera {
    _camera = camera;
    [self setCameraIfAvailable];
}

- (void)removeCamera:(VRTCamera *)camera {
    if (_camera != camera) {
        return;
    }
    if (_vroView) {
        [_vroView setPointOfView:nullptr];
    }
    _camera = nil;
}

#pragma mark - Scene-specific subviews

- (void)insertReactSubview:(UIView *)view atIndex:(NSInteger)atIndex {
    [super insertReactSubview:view atIndex:atIndex];
}

- (void)removeReactSubview:(UIView *)subview {
    [super removeReactSubview:subview];
}

#pragma mark - VROEventDelegateiOS implementation

- (void)onCameraTransformUpdate:(VROVector3f)position rotation:(VROVector3f)rotation forward:(VROVector3f)forward up:(VROVector3f)up {
    if (self.onCameraTransformUpdateViro) {
        NSArray<NSNumber *> * camTransform = [self cameraOrientation];
        self.onCameraTransformUpdateViro(@{kCameraTransform:camTransform});

    }
}

#pragma mark - Property setter overrides

/*
 * Grab the orientation of the current active camera within this scene.
 */
- (NSArray<NSNumber *> *)cameraOrientation {
    if (self.context) {
        VROVector3f cameraPosition = self.context->getCamera().getPosition();
        VROVector3f cameraRotation = self.context->getCamera().getRotation().toEuler();
        VROVector3f cameraForward = self.context->getCamera().getForward();
        VROVector3f cameraUp = self.context->getCamera().getUp();
        return @[@(cameraPosition.x), @(cameraPosition.y), @(cameraPosition.z),
                 @(toDegrees(cameraRotation.x)), @(toDegrees(cameraRotation.y)), @(toDegrees(cameraRotation.z)),
                 @(cameraForward.x), @(cameraForward.y), @(cameraForward.z),
                 @(cameraUp.x), @(cameraUp.y), @(cameraUp.z)];
    } else {
        return @[@0, @0, @0, @0, @0, @0, @0, @0, @0, @0, @0, @0];
    }
}

#pragma mark - VROSceneDelegateiOS methods.

- (void)sceneWillAppear:(VRORenderContext *)context driver:(std::shared_ptr<VRODriver>)driver {
    // the following lines call the setters which recursively set the context,
    // driver and scene down the entire scene tree.
    self.context = context;
    self.driver = driver;
    self.scene = self.scene; // this call self setScene: which passes the vroScene down the scene graph
    
    // we need to reset effects (override the ones from the previous scene) when we appear.
    self.scene->setPostProcessingEffectsUpdated(true);
    
    [self sceneWillAppear];
    [self parentDidAppear];

}

- (void)sceneDidAppear:(VRORenderContext *)context driver:(std::shared_ptr<VRODriver>)driver {
    
}

- (void)sceneWillDisappear:(VRORenderContext *)context driver:(std::shared_ptr<VRODriver>)driver {
    self.driver = driver;
    for(VRTView *view in _childViews) {
        [view sceneWillDisappear];
    }
}

- (void)sceneDidDisappear:(VRORenderContext *)context driver:(std::shared_ptr<VRODriver>)driver {
    
}

- (void)sceneWillRender:(const VRORenderContext *)context {
    self.context = (VRORenderContext *)context;
}

- (void)setPhysicsWorld:(NSDictionary *)dictionary{
    std::shared_ptr<VROPhysicsWorld> physicsWorld = [self scene]->getPhysicsWorld();
    NSArray *gravity = [dictionary objectForKey:@"gravity"];
    if (gravity != nil){
        if ([gravity count] !=3){
            RCTLogError(@"Insufficient parameters provided for gravity, expected: [x, y, z]!");
        } else {
            VROVector3f gravity3f = VROVector3f([[gravity objectAtIndex:0] floatValue],
                                                [[gravity objectAtIndex:1] floatValue],
                                                [[gravity objectAtIndex:2] floatValue]);
            physicsWorld->setGravity(gravity3f);
        }
    }
    
    bool enabled = [[dictionary objectForKey:@"drawBounds"] boolValue];
    physicsWorld->setDebugDrawVisible(enabled);
}

- (void)setPostProcessEffects:(NSArray<NSString *> *)effects {
    std::vector<std::string> strEffects;
    for (int i = 0; i < [effects count]; i++) {
        NSString *effect = [effects objectAtIndex:i];
        std::string strEffect = std::string([effect UTF8String]);
        strEffects.push_back(strEffect);
    }
    self.scene->setPostProcessingEffects(strEffects);
}

- (void)setCanCameraTransformUpdate:(BOOL)canCameraTransformUpdate {
    _canCameraTransformUpdate = canCameraTransformUpdate;
    self.eventDelegate->setEnabledEvent(VROEventDelegate::EventAction::OnCameraTransformUpdate, canCameraTransformUpdate);
}

/**
 * Log out debug information specific to this scene, such as Camera forward vector
 * in cartesian and polar formats.
 **/
- (void)logSceneDebugInfo:(const VRORenderContext *)context{
    VROVector3f forwardVector = context->getCamera().getForward().normalize();
    float r = sqrtf((pow(forwardVector.x, 2)) + (pow(forwardVector.y, 2))+ (pow(forwardVector.z, 2)));
    float theta = toDegrees(atan(forwardVector.z / forwardVector.x));
    float phi = toDegrees(acos(forwardVector.y / r));
    
    /* Required offsets because of the Right Hand Rule used for our coordinate system for Theta.
     * Currently, in the XZ plane, the observed vector and theta angles in in the following format:
     *          (-z)
     *        90 | -90
     * 0         |        0
     * (-x) - - - - - - (+x)
     * 0         |        0
     *        -90| 90
     *          (+z)
     * As such, we'll need to apply the following offsets:
     */
    if (forwardVector.x < 0){
        theta = theta + 180;
    } else if (forwardVector.z < 0){
        theta = theta + 360;
    }
    
    NSString *log = [NSString stringWithFormat:
                     @"\nCamera Polar Coordinates: Theta %f, Phi %f. \nCamera Forward Vector %f, %f, %f",
                     theta, phi, forwardVector.x, forwardVector.y, forwardVector.z];
    [VRTLog debug:log];
}

@end
