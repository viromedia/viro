//
//  VROSceneController.h
//  React
//
//  Created by Vik Advani on 4/4/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTSceneDelegate.h"

@interface RCTSceneController : VROSceneController

@property (nonatomic, weak, nullable) id<VRTSceneDelegate> sceneDelegate;

- (instancetype)initWithView:(id <VROView>)view sceneDelegate:(id<VRTSceneDelegate>)delegate;

@end
