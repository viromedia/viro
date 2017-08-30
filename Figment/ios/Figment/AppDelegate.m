/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import "AppDelegate.h"

#include "VRTBundleURLProvider.h"
#include "RCTBundleURLProvider.h"
#include "RCTRootView.h"

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
  BOOL enterVrImmediately = YES;
  BOOL usingNgrok = YES;

  if(enterVrImmediately) {
    NSURL *jsCodeLocation = nil;
#ifdef DEBUG
    if(usingNgrok) {
      VRTBundleURLProvider *bundleProvider = [[VRTBundleURLProvider alloc] init];
      jsCodeLocation = [bundleProvider jsBundleURLForBundleRoot:@"index.ios" fallbackResource:nil];
    }
#endif
    if(jsCodeLocation == nil) {
      jsCodeLocation = [[RCTBundleURLProvider sharedSettings] jsBundleURLForBundleRoot:@"index.ios" fallbackResource:nil];
    }

    RCTRootView *rootView = [[RCTRootView alloc] initWithBundleURL:jsCodeLocation
                                                      moduleName:@"Figment"
                                               initialProperties:nil
                                                   launchOptions:launchOptions];

    rootView.backgroundColor = UIColor.blackColor;
    
    // If we want a splash image, then we also need to set the image to the background b/c the RCTRootView
    // appears while we're loading and processing the bundle too.
//    UIImageView *imageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"viro_loading.png"]];
//    [rootView addSubview:imageView];
//    [rootView sendSubviewToBack:imageView];
//    [imageView setFrame:[UIScreen mainScreen].bounds];
//    imageView.contentMode = UIViewContentModeScaleToFill;
    
    self.window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    UIViewController *rootViewController = [UIViewController new];
    rootViewController.view = rootView;
    self.window.rootViewController = rootViewController;
    [self.window makeKeyAndVisible];
  }
  return YES;
}

@end
