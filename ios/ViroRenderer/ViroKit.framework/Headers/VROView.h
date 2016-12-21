//
//  VROView.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/15/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "VRORenderDelegate.h"
#import "VROQuaternion.h"
#import "VROCamera.h"
#import <memory>

@class VROSceneController;
class VROScene;
class VROReticle;
class VROFrameSynchronizer;
enum class VROTimingFunctionType;

typedef void (^VROViewValidApiKeyBlock)(BOOL);

@protocol VROView <NSObject>

@required

@property (nonatomic, unsafe_unretained) IBOutlet id <VRORenderDelegate> renderDelegate;
@property (nonatomic, readonly) std::shared_ptr<VROReticle> reticle;
@property (readwrite, nonatomic) VROSceneController *sceneController;

- (void)setSceneController:(VROSceneController *)sceneController animated:(BOOL)animated;
- (void)setSceneController:(VROSceneController *)sceneController duration:(float)seconds
            timingFunction:(VROTimingFunctionType)timingFunctionType;

- (void)setPosition:(VROVector3f)position;
- (void)setBaseRotation:(VROQuaternion)rotation;
- (void)setCameraRotationType:(VROCameraRotationType)type;
- (void)setOrbitFocalPoint:(VROVector3f)focalPt;

- (void)validateApiKey:(NSString *)apiKey withCompletionBlock:(VROViewValidApiKeyBlock)completionBlock;

/*
 Calling setVrMode allows switching to and from VR mode.
 When set to NO, it transitions back to pre-VR (mono) mode.
 When set to YES, we set thie view into a full VR mode
 */
- (void)setVrMode:(BOOL)enabled;

- (std::shared_ptr<VROFrameSynchronizer>)frameSynchronizer;

@end
