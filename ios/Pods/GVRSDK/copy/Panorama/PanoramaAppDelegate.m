#import "PanoramaAppDelegate.h"

#import "PanoramaViewController.h"

@implementation PanoramaAppDelegate

- (BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
  UIViewController *viewController = [[PanoramaViewController alloc] init];
  viewController = [[UINavigationController alloc] initWithRootViewController:viewController];

  self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
  self.window.rootViewController = viewController;
  [self.window makeKeyAndVisible];
  return YES;
}

@end
