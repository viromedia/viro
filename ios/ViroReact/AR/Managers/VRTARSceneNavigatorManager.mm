//
//  VRTARSceneNavigatorManager.mm
//  ViroReact
//
//  Created by Andy Chu on 6/12/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <React/RCTUIManager.h>
#import <ViroKit/ViroKit.h>
#import "VRTARSceneNavigatorManager.h"
#import "VRTARSceneNavigator.h"

@implementation VRTARSceneNavigatorManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(currentSceneIndex, NSInteger)
RCT_EXPORT_VIEW_PROPERTY(apiKey, NSString)
RCT_EXPORT_VIEW_PROPERTY(worldAlignment, NSString)

- (VRTARSceneNavigator *)view
{
    return [[VRTARSceneNavigator alloc] initWithBridge:self.bridge];
}

@end
