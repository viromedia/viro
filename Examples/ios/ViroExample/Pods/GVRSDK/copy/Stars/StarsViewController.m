#import "StarsViewController.h"

#import "StarsRenderLoop.h"
#import "StarsRenderer.h"

@interface StarsViewController () {
  GVRCardboardView *_cardboardView;
  StarsRenderer *_starsRenderer;
  StarsRenderLoop *_renderLoop;
}
@end

@implementation StarsViewController

- (void)loadView {
  _starsRenderer = [[StarsRenderer alloc] init];

  _cardboardView = [[GVRCardboardView alloc] initWithFrame:CGRectZero];
  _cardboardView.delegate = _starsRenderer;
  _cardboardView.autoresizingMask =
      UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;

  _cardboardView.vrModeEnabled = YES;

  // Use double-tap gesture to toggle between VR and magic window mode.
  UITapGestureRecognizer *doubleTapGesture =
      [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(didDoubleTapView:)];
  doubleTapGesture.numberOfTapsRequired = 2;
  [_cardboardView addGestureRecognizer:doubleTapGesture];

  self.view = _cardboardView;
}

- (void)viewWillAppear:(BOOL)animated {
  [super viewWillAppear:animated];

  _renderLoop = [[StarsRenderLoop alloc] initWithRenderTarget:_cardboardView
                                                     selector:@selector(render)];
  _starsRenderer.renderLoop = _renderLoop;
}

- (void)viewDidDisappear:(BOOL)animated {
  [super viewDidDisappear:animated];

  [_renderLoop invalidate];
  _renderLoop = nil;
}

#pragma mark - Implementation

- (void)didDoubleTapView:(id)sender {
  _cardboardView.vrModeEnabled = !_cardboardView.vrModeEnabled;
}

@end
