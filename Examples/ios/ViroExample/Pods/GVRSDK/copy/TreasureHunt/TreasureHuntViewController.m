#import "TreasureHuntViewController.h"

#import "TreasureHuntRenderLoop.h"
#import "TreasureHuntRenderer.h"

@interface TreasureHuntViewController ()<TreasureHuntRendererDelegate> {
  GVRCardboardView *_cardboardView;
  TreasureHuntRenderer *_treasureHuntRenderer;
  TreasureHuntRenderLoop *_renderLoop;
}
@end

@implementation TreasureHuntViewController

- (void)loadView {
  _treasureHuntRenderer = [[TreasureHuntRenderer alloc] init];
  _treasureHuntRenderer.delegate = self;

  _cardboardView = [[GVRCardboardView alloc] initWithFrame:CGRectZero];
  _cardboardView.delegate = _treasureHuntRenderer;
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

  _renderLoop = [[TreasureHuntRenderLoop alloc] initWithRenderTarget:_cardboardView
                                                            selector:@selector(render)];
}

- (void)viewDidDisappear:(BOOL)animated {
  [super viewDidDisappear:animated];

  // Invalidate the render loop so that it removes the strong reference to cardboardView.
  [_renderLoop invalidate];
  _renderLoop = nil;
}

- (GVRCardboardView *)getCardboardView {
  return _cardboardView;
}

#pragma mark - TreasureHuntRendererDelegate

- (void)shouldPauseRenderLoop:(BOOL)pause {
  _renderLoop.paused = pause;
}

#pragma mark - Implementation

- (void)didDoubleTapView:(id)sender {
  _cardboardView.vrModeEnabled = !_cardboardView.vrModeEnabled;
}

@end
