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
@property (readwrite, nonatomic) std::shared_ptr<VROPortalTraversalListener> portalTraversalListener;


- (instancetype)initWithBridge:(RCTBridge *)bridge;
-(void)addTraversalListenerToARView;
-(void)removeTraversalListenerFromARView;

- (void)initSceneController;

- (void)insertReactSubview:(UIView *)view atIndex:(NSInteger)atIndex;

@end
