#import <UIKit/UIKit.h>

#import "VideoPlayerViewController.h"

#import "GVRVideoView.h"

@interface VideoPlayerViewController () <GVRVideoViewDelegate>
@property(nonatomic) IBOutlet GVRVideoView *videoView;
@property(nonatomic) IBOutlet UITextView *attributionTextView;
@end

@implementation VideoPlayerViewController {
  BOOL _isPaused;
}

- (instancetype)init {
  self = [super initWithNibName:nil bundle:nil];
  return self;
}

- (void)viewDidLoad {
  [super viewDidLoad];

  // Build source attribution text view.
  NSString *sourceText = @"Source: ";
  NSMutableAttributedString *attributedText = [[NSMutableAttributedString alloc]
      initWithString:[sourceText stringByAppendingString:@"Wikipedia"]];
  [attributedText
      addAttribute:NSLinkAttributeName
             value:@"https://en.wikipedia.org/wiki/Gorilla"
             range:NSMakeRange(sourceText.length, attributedText.length - sourceText.length)];

  _attributionTextView.attributedText = attributedText;

  _videoView.delegate = self;
  _videoView.enableFullscreenButton = YES;
  _videoView.enableCardboardButton = YES;
  _videoView.enableTouchTracking = YES;

  _isPaused = NO;

  // Load the sample 360 video, which is of type stereo-over-under.
  NSString *videoPath = [[NSBundle mainBundle] pathForResource:@"congo" ofType:@"mp4"];
  [_videoView loadFromUrl:[[NSURL alloc] initFileURLWithPath:videoPath]
                   ofType:kGVRVideoTypeStereoOverUnder];
}

#pragma mark - GVRVideoViewDelegate

- (void)widgetViewDidTap:(GVRWidgetView *)widgetView {
  if (_isPaused) {
    [_videoView resume];
  } else {
    [_videoView pause];
  }
  _isPaused = !_isPaused;
}

- (void)widgetView:(GVRWidgetView *)widgetView didLoadContent:(id)content {
  NSLog(@"Finished loading video");
}

- (void)widgetView:(GVRWidgetView *)widgetView
    didFailToLoadContent:(id)content
        withErrorMessage:(NSString *)errorMessage {
  NSLog(@"Failed to load video: %@", errorMessage);
}

- (void)videoView:(GVRVideoView*)videoView didUpdatePosition:(NSTimeInterval)position {
  // Loop the video when it reaches the end.
  if (position == videoView.duration) {
    [_videoView seekTo:0];
    [_videoView resume];
  }
}

@end
