/**
 * @file GVRVideoView.h
 * @brief GVRVideoView.h File
 */

#import "GVRWidgetView.h"

/**
 * @enum GVRVideoType
 * Enum for video image types.
 */
typedef NS_ENUM(int, GVRVideoType) {
  /**
   * Each video frame is a monocular equirectangular panorama.
   * Each frame image is expected to cover 360 degrees along its horizontal axis.
   */
  kGVRVideoTypeMono = 1,

  /**
   * Each video frame contains two vertically-stacked equirectangular panoramas. The top part of
   * the frame contains pixels for the left eye, while the bottom part of the frame contains
   * pixels for the right eye.
   */
  kGVRVideoTypeStereoOverUnder,

  /**
   * The video contains metadata in the spherical v2 format which describes how
   * to render it. See
   * https://github.com/google/spatial-media/blob/master/docs/spherical-video-v2-rfc.md.
   */
  kGVRVideoTypeSphericalV2,
};

/**
 * Defines a player view that renders a 360 video using OpenGL.
 */
@interface GVRVideoView : GVRWidgetView

/**
 * Load a local or remote video from a url and start playing.
 *
 * The video is assumed to be of type ::kGVRVideoTypeMono.
 */
- (void)loadFromUrl:(NSURL*)videoUrl;

/**
 * Load a local or remote video from a url and start playing.
 *
 * The video type is set by @c videoType.
 */
- (void)loadFromUrl:(NSURL*)videoUrl ofType:(GVRVideoType)videoType;

/** Pause the video. */
- (void)pause;

/** Start or resume the video. */
- (void)play;

/** Stop the video. */
- (void)stop;

/** Get the duration of the video. */
- (NSTimeInterval)duration;

/** Gets the playable duration of the buffered video. */
- (NSTimeInterval)playableDuration;

/** Seek to the target time position of the video. */
- (void)seekTo:(NSTimeInterval)position;

/**
 * Defines a volume multiplier between 0.0f and 1.0f on the media audio playback. This setting
 * persists across multiple videos. 0.0f means muted, while 1.0f means regular volume.
 * Default value is 1.0f.
 */
@property(nonatomic) float volume;

@end

/** Defines a protocol to notify delegates of change in video playback. */
@protocol GVRVideoViewDelegate <GVRWidgetViewDelegate>

/** Called when position of the video playback head changes. */
- (void)videoView:(GVRVideoView*)videoView didUpdatePosition:(NSTimeInterval)position;

@end
