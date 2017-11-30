#import <UIKit/UIKit.h>

/**
 * Defines a delegate protocol for |GVROverlayView| class. It's used to respond to the user
 * tapping various controls in the overlay view.
 */
@protocol GVROverlayViewDelegate <NSObject>

@optional

/** Called when the user tapped on the GL rendering view. */
- (void)didTapTriggerButton;

/** Called when the user tapped the Back arrow button. */
- (void)didTapBackButton;

/** Called when the user tapped the Cardboard icon button. */
- (void)didTapCardboardButton;

/**
 * Called to get a presenting view controller to present the settings dialog. If this is not
 * implemented, the presenting view controller is derived from the applications rootViewController.
 */
- (UIViewController *)presentingViewControllerForSettingsDialog;

/**
 * Called when the setting dialog is presented. Delegates can use this to pause/resume GL rendering.
 */
- (void)didPresentSettingsDialog:(BOOL)presented;

/** Called when the VR viewer is paired by the user. Delegates should refresh the GL rendering. */
- (void)didChangeViewerProfile;

/**
 * Called to allow the delegate to manage their own application idle timer. If not implemented, it
 * is managed by the SDK.
 */
- (void)shouldDisableIdleTimer:(BOOL)shouldDisable;

@end


/**
 * Defines a view to display various overlay controls on top of |GVRCardboardView|.
 */
@interface GVROverlayView : UIView

@property(nonatomic, weak) id<GVROverlayViewDelegate> delegate;

@property(nonatomic) BOOL hidesAlignmentMarker;
@property(nonatomic) BOOL hidesBackButton;
@property(nonatomic) BOOL hidesCardboardButton;
@property(nonatomic) BOOL hidesFullscreenButton;
@property(nonatomic) BOOL hidesSettingsButton;
@property(nonatomic) BOOL hidesTransitionView;
@property(nonatomic) BOOL hidesWatermark;

@end
