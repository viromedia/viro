//
//  VRT3DSceneNavigatorManager.mm
//  ViroReact
//
//  Created by Manish Bodhankar on 3/12/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#import <React/RCTUIManager.h>
#import "VRT3DSceneNavigatorManager.h"
#import "VRT3DSceneNavigator.h"


@implementation VRT3DSceneNavigatorManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(currentSceneIndex, NSInteger)
RCT_EXPORT_VIEW_PROPERTY(apiKey, NSString)
RCT_EXPORT_VIEW_PROPERTY(onExitViro, RCTDirectEventBlock)

- (VRT3DSceneNavigator *)view
{
  return [[VRT3DSceneNavigator alloc] initWithBridge:self.bridge];
}

RCT_EXPORT_METHOD(requestExit:(nonnull NSNumber *)reactTag)
{
  [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
    VRTView *view = (VRTView *)viewRegistry[reactTag];
    if (![view isKindOfClass:[VRT3DSceneNavigator class]]) {
      RCTLogError(@"Invalid view returned from registry, expecting VRT3DSceneNavigator, got: %@", view);
    } else {
      VRT3DSceneNavigator *component = (VRT3DSceneNavigator *)view;
      [component userDidRequestExitVR];
    }
  }];
}

@end
