//
//  VRTARScene.h
//  ViroReact
//
//  Created by Andy Chu on 6/13/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTScene.h"

@interface VRTARScene : VRTScene <VROARSceneDelegateProtocol>

@property (nonatomic, copy, nullable) RCTDirectEventBlock onTrackingInitializedViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onAmbientLightUpdateViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onAnchorFoundViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onAnchorUpdatedViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onAnchorRemovedViro;

- (instancetype)initWithBridge:(RCTBridge *)bridge;

- (void)initSceneController;

- (void)insertReactSubview:(UIView *)view atIndex:(NSInteger)atIndex;

@end
