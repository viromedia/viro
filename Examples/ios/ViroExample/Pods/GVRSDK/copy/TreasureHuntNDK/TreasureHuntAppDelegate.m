/*
 * Copyright 2016 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support. Compile with -fobjc-arc"
#endif

#import "TreasureHuntAppDelegate.h"

#import "TreasureHuntViewController.h"

@interface TreasureHuntAppDelegate ()<UINavigationControllerDelegate>

@end

@implementation TreasureHuntAppDelegate

#pragma mark - UIApplicationDelegate overrides

- (BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
  UINavigationController *navigationController = [[UINavigationController alloc]
                          initWithRootViewController:[[TreasureHuntViewController alloc] init]];
  navigationController.delegate = self;
  navigationController.navigationBarHidden = YES;

  self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
  self.window.rootViewController = navigationController;
  [self.window makeKeyAndVisible];
  return YES;
}

#pragma mark - UINavigationControllerDelegate

// Make the navigation controller defer the check of supported orientation to its topmost view
// controller. This allows |GVRCardboardViewController| to lock the orientation in VR mode.
- (UIInterfaceOrientationMask)navigationControllerSupportedInterfaceOrientations:
                                  (UINavigationController *)navigationController {
  return [navigationController.topViewController supportedInterfaceOrientations];
}

@end
