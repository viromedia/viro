#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support. Compile with -fobjc-arc"
#endif

#import "StarsAppDelegate.h"

#import "StarsViewController.h"

@interface StarsAppDelegate ()<UINavigationControllerDelegate>

@end

@implementation StarsAppDelegate

#pragma mark - UIApplicationDelegate overrides

- (BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
  UINavigationController *navigationController = [[UINavigationController alloc]
      initWithRootViewController:[[StarsViewController alloc] init]];
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
