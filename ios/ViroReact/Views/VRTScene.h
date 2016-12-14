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
#import "VRTSceneDelegate.h"

@interface VRTScene : VRTNode<VRTSceneDelegate>

@property (nonatomic, readonly) VROSceneController *sceneController;
@property (nonatomic, assign) BOOL recticleEnabled;

- (instancetype)initWithBridge:(RCTBridge *)bridge;
- (std::shared_ptr<VROScene>)scene;
- (NSArray<NSNumber *> *)cameraPosition;

- (void)setView:(id <VROView>)view;

@end
