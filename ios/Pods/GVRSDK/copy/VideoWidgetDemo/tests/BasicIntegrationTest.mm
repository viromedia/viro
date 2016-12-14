#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>

#import "googlemac/iPhone/CardboardSDK/Examples/EarlGreyUtils/GVREarlGreyTestCase.h"
#import "googlemac/iPhone/CardboardSDK/Source/GVRLocalization.h"
#import "googlemac/iPhone/CardboardSDK/Source/GVROverlayView.h"
#import "googlemac/iPhone/CardboardSDK/Source/GVRVideoView.h"
#import "googlemac/iPhone/Shared/EarlGrey/EarlGrey.h"

@interface BasicIntegrationTest : GVREarlGreyTestCase
@end

@implementation BasicIntegrationTest {
  int _waitTimeSeconds;
}

- (void)testStartup {
  _waitTimeSeconds = 2;
  [self waitForSeconds:_waitTimeSeconds];

  // Swipe on display to move pano image.
  [[[EarlGrey findElementWithMatcher:eg_kindOfClass([GVRVideoView class])]
      checkWithMatcher:eg_displayedForInteraction()]
      performAction:eg_swipeSlowInDirectionWithStartPoint(kEGDirectionLeft,
                                                          0.05f, 0.3f)];

  // Check help button is displayed in GVRVideoView.
  [[self getHelpButton:[GVRVideoView class]]
      checkWithMatcher:eg_displayedForInteraction()];

  // Check cardboard button is displayed in GVRVideoView, then tap.
  [[[self getCardboardButton:[GVRVideoView class]]
      checkWithMatcher:eg_displayedForInteraction()] performAction:eg_tap()];

  // Rotate left to enter cardboard mode.
  [EarlGrey rotateDeviceToOrientation:UIDeviceOrientationLandscapeLeft];

  // Check GVROverlayView is displayed.
  [[EarlGrey findElementWithMatcher:eg_kindOfClass([GVROverlayView class])]
      checkWithMatcher:eg_displayedForInteraction()];

  [self waitForSeconds:_waitTimeSeconds];

  // Tap back button.
  [[EarlGrey findElementWithMatcher:eg_allOf(
      eg_accessibilityID(@"overlay_back_button"),
      eg_accessibilityLabel([GVRLocalization stringForId:kStrAccessibilityBackButton]),
      eg_displayedForInteraction(), nil)]
    performAction:eg_tap()];

  [self waitForSeconds:_waitTimeSeconds];

  // Check GVRPanoramaView is displayed.
  [[EarlGrey findElementWithMatcher:eg_kindOfClass([GVRVideoView class])]
      checkWithMatcher:eg_displayedForInteraction()];

  // Rotate back to portrait
  [EarlGrey rotateDeviceToOrientation:UIDeviceOrientationPortrait];
}

@end
