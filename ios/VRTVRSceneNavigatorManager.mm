//
//  VRTVRSceneNavigatorManager.m
//  ViroReact
//
//  Created by Manish Bodhankar on 3/30/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#import <React/RCTUIManager.h>
#import "VRTVRSceneNavigatorManager.h"
#import "VRTVRSceneNavigator.h"


@implementation VRTVRSceneNavigatorManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(currentSceneIndex, NSInteger)
RCT_EXPORT_VIEW_PROPERTY(vrModeEnabled, BOOL)
RCT_EXPORT_VIEW_PROPERTY(onExitViro, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(hdrEnabled, BOOL)
RCT_EXPORT_VIEW_PROPERTY(pbrEnabled, BOOL)
RCT_EXPORT_VIEW_PROPERTY(bloomEnabled, BOOL)
RCT_EXPORT_VIEW_PROPERTY(shadowsEnabled, BOOL)
RCT_EXPORT_VIEW_PROPERTY(multisamplingEnabled, BOOL)

- (VRTVRSceneNavigator *)view
{
  return [[VRTVRSceneNavigator alloc] initWithBridge:self.bridge];
}

RCT_EXPORT_METHOD(requestExit:(nonnull NSNumber *)reactTag)
{
  [self.bridge.uiManager addUIBlock:^(__unused RCTUIManager *uiManager, NSDictionary<NSNumber *, UIView *> *viewRegistry) {
    VRTView *view = (VRTView *)viewRegistry[reactTag];
    if (![view isKindOfClass:[VRTVRSceneNavigator class]]) {
      RCTLogError(@"Invalid view returned from registry, expecting VRTSceneNavigator, got: %@", view);
    } else {
      VRTVRSceneNavigator *component = (VRTVRSceneNavigator *)view;
      [component userDidRequestExitVR];
    }
  }];
}

@end

