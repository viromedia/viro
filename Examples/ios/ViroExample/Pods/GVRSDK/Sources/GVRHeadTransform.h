/**
 * @file GVRHeadTransform.h
 * @brief GVRHeadTransform.h File
 */

#import <GLKit/GLKit.h>

/**
 * @enum GVREye
 * Enumeration of the left and right eyes, used to identify the correct
 * rendering parameters needed for stereo rendering.
 */
typedef NS_ENUM(NSInteger, GVREye) {
  kGVRLeftEye,    /**< Left eye. */
  kGVRRightEye,   /**< Right eye. */
  kGVRCenterEye,  /**< Center eye. */
};

/**
 * Defines a struct to hold half field of view angles, in degrees, for an @c GVREye eye.
 */
typedef struct {
  CGFloat left;
  CGFloat right;
  CGFloat top;
  CGFloat bottom;
} GVRFieldOfView;

/**
 * Defines a class to represent the head transformation for a render frame.
 */
@interface GVRHeadTransform : NSObject

/** Returns the screen viewport for a given eye. */
- (CGRect)viewportForEye:(GVREye)eye;

/** Returns the projection matrix for the specified eye. */
- (GLKMatrix4)projectionMatrixForEye:(GVREye)eye near:(CGFloat)near far:(CGFloat)far;

/**
 * Returns the transformation matrix used to convert from Head Space to Eye Space for the given
 * eye.
 */
- (GLKMatrix4)eyeFromHeadMatrix:(GVREye)eye;

/** Returns the reference matrix of the head pose in start space. */
- (GLKMatrix4)headPoseInStartSpace;

/** Returns the field of view for the specified eye. */
- (GVRFieldOfView)fieldOfViewForEye:(GVREye)eye;

@end
