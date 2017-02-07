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

class VROSceneController;
class VROScene;
class VROReticle;
class VRONode;
class VROFrameSynchronizer;
enum class VROTimingFunctionType;

typedef void (^VROViewValidApiKeyBlock)(BOOL);

@protocol VROView <NSObject>

@required

@property (nonatomic, unsafe_unretained) IBOutlet id <VRORenderDelegate> renderDelegate;
@property (readwrite, nonatomic) std::shared_ptr<VROSceneController> sceneController;

- (void)setSceneController:(std::shared_ptr<VROSceneController>)sceneController animated:(BOOL)animated;
- (void)setSceneController:(std::shared_ptr<VROSceneController>)sceneController duration:(float)seconds
            timingFunction:(VROTimingFunctionType)timingFunctionType;

- (void)setPointOfView:(std::shared_ptr<VRONode>)node;
- (void)validateApiKey:(NSString *)apiKey withCompletionBlock:(VROViewValidApiKeyBlock)completionBlock;

/*
 Calling setVrMode allows switching to and from VR mode.
 When set to NO, it transitions back to pre-VR (mono) mode.
 When set to YES, we set thie view into a full VR mode
 */
- (void)setVrMode:(BOOL)enabled;

- (std::shared_ptr<VROFrameSynchronizer>)frameSynchronizer;

@end
