/** @file GVRCardboardView.h
 *  @brief GVRCardboardView.h File
 */

#import <UIKit/UIKit.h>

#import "GVRHeadTransform.h"

/**
 * @enum GVRUserEvent
 * Cardboard specific user events.
 */
typedef NS_ENUM(NSInteger, GVRUserEvent) {
  /**
   * Back event triggered when the user taps the back button icon located at the top left of the
   * screen. This is generally used to 'exit' Cardboard mode and go back to previous 2D content.
   * This event is fired on the main thread.
   */
  kGVRUserEventBackButton,

  /**
   * Event triggered when Cardboard is sharply tilted to the side. This is generally used to
   * 'go back' while the phone is in Cardboard and the user is still in Cardboard mode. This event
   * is fired on the GL thread.
   */
  kGVRUserEventTilt,

  /**
   * Cardboard trigger event, generally fired due to a magnet pull or button press. This event is
   * fired on the GL thread.
   */
  kGVRUserEventTrigger,
};

@class GVRCardboardView;

/**
 * Defines a delegate protocol for @c GVRCardboardView.
 */
@protocol GVRCardboardViewDelegate<NSObject>

@optional

/**
 * Called when a user event is fired. See the documentation of #GVRUserEvent to find out what
 * thread is used to make this call.
 */
- (void)cardboardView:(GVRCardboardView *)cardboardView didFireEvent:(GVRUserEvent)event;

/**
 * Called before the first draw frame call. This is called on the GL thread and can be used to do
 * any pre-rendering setup required while on the GL thread.
 */
- (void)cardboardView:(GVRCardboardView *)cardboardView
     willStartDrawing:(GVRHeadTransform *)headTransform;

/**
 * Called at the start of each frame, before calling both eyes. Delegate should use initialize or
 * clear the GL state. This method is called on the GL thread.
 */
- (void)cardboardView:(GVRCardboardView *)cardboardView
     prepareDrawFrame:(GVRHeadTransform *)headTransform;

@required

/**
 * Called on each frame to perform the required GL rendering. Delegate should set the GL viewport
 * and scissor it to the viewport returned from the GVRHeadTransform::viewportForEye: method.
 * This method is called on the GL thread.
 */
- (void)cardboardView:(GVRCardboardView *)cardboardView
              drawEye:(GVREye)eye
    withHeadTransform:(GVRHeadTransform *)headTransform;

@optional

/**
 * Called when the drawing is paused, possibly because the GL view is overlaid by settings or
 * transition view. This is called on the main UI thread. Drawing is stopped when the parameter
 * paused is set to YES and resumes when set to NO.
 */
- (void)cardboardView:(GVRCardboardView *)cardboardView shouldPauseDrawing:(BOOL)pause;

@end

/**
 * Defines a view responsible for rendering graphics in VR mode. It is designed
 * to work in full screen mode with a landscape orientation. It provides all
 * the transition between mono and stereo (VR) modes. The view switches to
 * fullscreen VR mode when @c #vrModeEnabled property is set to YES. The developer
 * should ensure @c UISupportedInterfaceOrientations key in the application's plist
 * file includes @c UIInterfaceOrientationPortrait and @c UIRequiresFullscreen is
 * set to YES.
 *
 * The users of this class provide an implementation of the
 * @c GVRCardboardViewDelegate protocol through the @c delegate property. The
 * methods in the protocol @c GVRCardboardViewDelegate are called to perform
 * the actual rendering of the graphics.
 *
 * In order to drive the rendering, the users of this class should call the
 * @c render method from their render loop. The thread used to call the
 * @c render method is refered to as the GL thread. This thread, in turn, calls
 * the method of the @c GVRCardboardViewDelegate protocol.
 */
@interface GVRCardboardView : UIView

/** Designated initializer. */
- (instancetype)initWithFrame:(CGRect)frame NS_DESIGNATED_INITIALIZER;

/** Delegate to be notified of cardboard view rendering. */
@property(nonatomic, weak) id<GVRCardboardViewDelegate> delegate;

/** The OpenGL context used for rendering. Defaults to OpenGL ES3 context, if not set. */
@property(nonatomic) EAGLContext *context;

/**
 * The vrModeEnabled property allows switching to and from VR mode. When set to YES, the cardboard
 * view transitions into a fullscreen view to allow the user to place the device into the Cardboard
 * viewer. When set to NO, it transitions back to pre-VR (mono) mode.
 */
@property(nonatomic) BOOL vrModeEnabled;

/** Flag to hide or show the transition view when in VR mode. */
@property(nonatomic) BOOL hidesTransitionView;

/** Drives the rendering of the cardboard view. Call this from your app's render loop. */
- (void)render;

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
