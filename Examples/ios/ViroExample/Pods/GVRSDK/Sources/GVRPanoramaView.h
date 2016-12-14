/**
 * @file GVRPanoramaView.h
 * @brief GVRPanoramaView.h File
 */

#import "GVRWidgetView.h"

/**
 * @enum GVRPanoramaImageType
 * The enum of panorama image types.
 */
typedef NS_ENUM(int, GVRPanoramaImageType) {
  /**
   * Regular image from a single image source, representing pixels for only one eye.
   * The image is expected to cover 360 degrees along its horizontal axis, while the
   * vertical range is computed according to the image's aspect ratio. For example, if a 1000x250
   * pixels image is given, the panorama will cover 360x90 degrees with the vertical range
   * being -45 to +45 degrees.
   */
  kGVRPanoramaImageTypeMono = 1,

  /**
   * Image in over-under format, from a single image source. Pixels in the upper half correspond to
   * the left eye. Pixels in the lower half correspond to the right eye.
   * The image is expected to cover 360 degrees along its horizontal axis, while the
   * vertical range is computed according to the image's aspect ratio. For example, if a 1000x500
   * pixels image is given (i.e 1000x250 pixels for each eye), the panorama will cover 360x90
   * degrees with the vertical range being -45 to +45 degrees.
   */
  kGVRPanoramaImageTypeStereoOverUnder,
};

/** Defines a view that can load and display 360-degree panoramic photos. */
@interface GVRPanoramaView : GVRWidgetView

/**
 * Load a 360-Panorama image from @c UIImage of type ::kGVRPanoramaImageTypeMono.
 *
 * If image is nil, it clears the view.
 */
- (void)loadImage:(UIImage *)image;

/**
 * Load a 360-Panorama image from @c UIImage of type ::GVRPanoramaImageType.
 *
 * If image is nil, it clears the view.
 */
- (void)loadImage:(UIImage *)image ofType:(GVRPanoramaImageType)imageType;

@end
