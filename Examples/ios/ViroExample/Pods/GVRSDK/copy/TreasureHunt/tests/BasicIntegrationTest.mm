#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>

#import "googlemac/iPhone/CardboardSDK/Examples/EarlGreyUtils/GVREarlGreyTestCase.h"
#import "googlemac/iPhone/CardboardSDK/Source/GVRCardboardView.h"
#import "googlemac/iPhone/CardboardSDK/Source/GVROverlayView.h"
#import "googlemac/iPhone/Shared/EarlGrey/EarlGrey.h"

static NSString *const switchString = @"SWITCH";
static NSString *const skipString = @"SKIP";
static NSString *const backButtonString = @"Back to previous screen";
static NSString *const settingsString = @"Settings";
static NSString *const settingsHelpString = @"Help";

@interface BasicIntegrationTest : GVREarlGreyTestCase
@end

@implementation BasicIntegrationTest {
  int _numberOfSwipes;
  int _waitTimeSeconds;
}

- (void)testStartup {
  _numberOfSwipes = 5;
  _waitTimeSeconds = 2;
  [[EarlGrey findElementWithMatcher:eg_buttonTitle(switchString)]
      performAction:eg_tap()];

  [[EarlGrey findElementWithMatcher:eg_buttonTitle(skipString)]
      performAction:eg_tap()];

  [EarlGrey rotateDeviceToOrientation:UIDeviceOrientationLandscapeLeft];

  // Check back button is displayed.
  [[self getDisplayedElementByAccessibilityLabel:backButtonString]
      checkWithMatcher:eg_displayedForInteraction()];

  [self waitForSeconds:_waitTimeSeconds];

  // Check GVRCardboardView is shown.
  [[EarlGrey findElementWithMatcher:eg_kindOfClass([GVRCardboardView class])]
      checkWithMatcher:eg_displayedForInteraction()];

  // Swipe on display to move cube.
  for (int i = 0; i < _numberOfSwipes; i++) {
    [[EarlGrey findElementWithMatcher:eg_kindOfClass([GVROverlayView class])]
        performAction:eg_swipeSlowInDirectionWithStartPoint(kEGDirectionDown,
                                                            0.05f, 0.05f)];
  }

  // Tap on Settings button.
  [[self getDisplayedElementByAccessibilityLabel:settingsString]
      performAction:eg_tap()];

  // Check help button is displayed.
  [[EarlGrey findElementWithMatcher:eg_buttonTitle(settingsHelpString)]
      checkWithMatcher:eg_displayedForInteraction()];
}

@end
