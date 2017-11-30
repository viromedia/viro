/**
 * @file GVRWidgetView.h
 * @brief GVRWidgetView.h File
 */

#import <UIKit/UIKit.h>

@protocol GVRWidgetViewDelegate;

/**
 * @enum GVRWidgetDisplayMode
 * The enum of various widget display modes.
 */
typedef NS_ENUM(NSInteger, GVRWidgetDisplayMode) {
  /** Widget is displayed embedded in other views. */
  kGVRWidgetDisplayModeEmbedded = 1,
  /** Widget is displayed in fullscreen mono mode. */
  kGVRWidgetDisplayModeFullscreen,
  /** Widget is displayed in fullscreen VR (stereo) mode. */
  kGVRWidgetDisplayModeFullscreenVR,
};

/**
 * Contains yaw and pitch angles corresponding to where the user is looking.
 *
 * `yaw` is the rotation along the vertical (y) axis. Values are in the
 * [-180, 180] range with:
 *
 * *  0 == looking straight at the initial image orientation
 * *  90 == looking 90 degrees to the right
 * *  -90 == looking 90 degrees to the left
 * *  180 == -180 == looking in the direction opposite to the initial one
 *
 * `pitch` is the rotation along the right (x) axis previously rotated by `yaw`.
 * Values are in the [-90, 90] range with:
 *
 * *  0 == looking straight, level with the ground
 * *  90 == looking up
 * *  -90 == looking down
 *
 * When `pitch` approaches 90 or -90, `yaw` values are reset to zero as
 * computing the yaw values becomes numerically unstable.
 */
typedef struct {
  CGFloat yaw;
  CGFloat pitch;
} GVRHeadRotation;

/** Defines a base class for all widget views, that encapsulates common functionality. */
@interface GVRWidgetView : UIView

/** The delegate that is called when the widget view is loaded. */
@property(nonatomic, weak) id<GVRWidgetViewDelegate> delegate;

/** Displays a button that allows the user to transition to fullscreen mode. */
@property(nonatomic) BOOL enableFullscreenButton;

/** Displays a button that allows the user to transition to fullscreen VR mode. */
@property(nonatomic) BOOL enableCardboardButton;

/** Displays a button that allows the user to get more information about VRView. */
@property(nonatomic) BOOL enableInfoButton;

/** Hides the transition view when entering VR mode. */
@property(nonatomic) BOOL hidesTransitionView;

/**
 * Enables touch gesture detection for manual heading control.
 * Regardless of how this is set, touch tracking is disabled in fullscreen VR mode.
 */
@property(nonatomic) BOOL enableTouchTracking;

/** The current head rotation corresponding to where the user is looking. */
@property(nonatomic, readonly) GVRHeadRotation headRotation;

/**
 * Controls the current ::GVRWidgetDisplayMode of the widget view.
 * Changing the value of this property is similar to pressing one of the fullscreen, cardboard
 * or back UI buttons.
 */
@property(nonatomic) GVRWidgetDisplayMode displayMode;

/**
 * Sets the Cardboard viewer parameters from a Url. This allows programmatically pairing a viewer
 * without having the user go through the pairing flow. Calls the supplied completion handler
 * with the result of pairing and the error if it fails.
 *
 * Note: The viewer parameters can be set only if no other viewer pairing exists. In other words,
 * this only works if the user hasn't already paired a viewer for the current application.
 *
 * A valid viewer parameters URI can be generated from this page:
 *     https://www.google.com/get/cardboard/viewerprofilegenerator/
 */
+ (void)setViewerParamsFromUrl:(NSURL *)url
                withCompletion:(void (^)(BOOL success, NSError *error))completion;

@end

/** Defines a delegate for GVRWidgetView and its subclasses. */
@protocol GVRWidgetViewDelegate<NSObject>

@optional

/**
 * Called when the user taps the widget view. This corresponds to the Cardboard viewer's trigger
 * event.
 */
- (void)widgetViewDidTap:(GVRWidgetView *)widgetView;

/** Called when the widget view's display mode changes. See ::GVRWidgetDisplayMode. */
- (void)widgetView:(GVRWidgetView *)widgetView
    didChangeDisplayMode:(GVRWidgetDisplayMode)displayMode;

/** Called when the content is successfully loaded. */
- (void)widgetView:(GVRWidgetView *)widgetView didLoadContent:(id)content;

/** Called when there is an error loading content in the widget view. */
- (void)widgetView:(GVRWidgetView *)widgetView
    didFailToLoadContent:(id)content
        withErrorMessage:(NSString *)errorMessage;

@end
