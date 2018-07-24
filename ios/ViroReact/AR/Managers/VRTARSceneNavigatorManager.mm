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
RCT_EXPORT_VIEW_PROPERTY(autofocus, BOOL)
RCT_EXPORT_VIEW_PROPERTY(videoQuality, NSString)
RCT_EXPORT_VIEW_PROPERTY(hdrEnabled, BOOL)
RCT_EXPORT_VIEW_PROPERTY(pbrEnabled, BOOL)
RCT_EXPORT_VIEW_PROPERTY(bloomEnabled, BOOL)
RCT_EXPORT_VIEW_PROPERTY(shadowsEnabled, BOOL)

- (VRTARSceneNavigator *)view
{
    return [[VRTARSceneNavigator alloc] initWithBridge:self.bridge];
}

@end
