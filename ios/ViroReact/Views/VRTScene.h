//
//  VRTScene.h
//  React
//
//  Created by Vik Advani on 12/11/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTNode.h"
#import "RCTBridge.h"
#import "VRTCamera.h"

@interface VRTScene : VRTNode<VROSceneDelegateProtocol>
@property (nonatomic, assign) BOOL recticleEnabled;

- (instancetype)initWithBridge:(RCTBridge *)bridge;
- (std::shared_ptr<VROScene>)scene;
- (std::shared_ptr<VROSceneController>)sceneController;
- (NSArray<NSNumber *> *)cameraPosition;

- (void)setView:(id <VROView>)view;

- (void)setCamera:(VRTCamera *)camera;
- (void)removeCamera:(VRTCamera *)camera;

@end
