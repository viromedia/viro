/*
 * Copyright 2016 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#import "TreasureHuntViewController.h"

#import "GVROverlayView.h"
#import "treasure_hunt_renderer.h"

@interface TreasureHuntViewController ()<GLKViewControllerDelegate, GVROverlayViewDelegate> {
  gvr_context *_gvrContext;
  std::unique_ptr<TreasureHuntRenderer> _renderer;
}
@end

@implementation TreasureHuntViewController

- (void)dealloc {
  gvr_destroy(&_gvrContext);
}

- (void)viewDidLoad {
  [super viewDidLoad];

  self.delegate = self;

  // Create an overlay view on top of the GLKView.
  GVROverlayView *overlayView = [[GVROverlayView alloc] initWithFrame:self.view.bounds];
  overlayView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
  overlayView.delegate = self;
  [self.view addSubview:overlayView];

  // Add a tap gesture to handle viewer trigger action.
  UITapGestureRecognizer *tapGesture =
      [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(didTapGLView:)];
  [self.view addGestureRecognizer:tapGesture];

  // Create an OpenGL ES context and assign it to the view loaded from storyboard
  GLKView *glkView = (GLKView *)self.view;
  glkView.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];

  // Set animation frame rate.
  self.preferredFramesPerSecond = 60;

  // Set the GL context to initialize GVR.
  [EAGLContext setCurrentContext:glkView.context];

  // Make sure the glkView has bound its offscreen buffers before calling into gvr.
  [glkView bindDrawable];

  // Create GVR context.
  _gvrContext = gvr_create();

  // Create GVR Audio context.
  std::unique_ptr<gvr::AudioApi> audio_context(new gvr::AudioApi);
  audio_context->Init(GVR_AUDIO_RENDERING_BINAURAL_HIGH_QUALITY);

  // Initialize TreasureHuntRenderer.
  _renderer.reset(new TreasureHuntRenderer(_gvrContext, std::move(audio_context)));
  _renderer->InitializeGl();
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations {
  // GVR only supports landscape right orientation for inserting the phone in the viewer.
  return UIInterfaceOrientationMaskLandscapeRight;
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
  _renderer->DrawFrame();
}

#pragma mark - GLKViewControllerDelegate

- (void)glkViewControllerUpdate:(GLKViewController *)controller {
  // Perform GL state update before drawing.
}

- (void)glkViewController:(GLKViewController *)controller willPause:(BOOL)pause {
  if (pause) {
    _renderer->OnPause();
  } else {
    _renderer->OnResume();
  }
}

#pragma mark - Actions

- (void)didTapGLView:(id)sender {
  _renderer->OnTriggerEvent();
}

#pragma mark - GVROverlayViewDelegate

- (void)didTapBackButton {
  // User pressed the back button. Pop this view controller.
  NSLog(@"User pressed back button");
}

- (UIViewController *)presentingViewControllerForSettingsDialog {
  return self;
}

- (void)didPresentSettingsDialog:(BOOL)presented {
  // The overlay view is presenting the settings dialog. Pause our rendering while presented.
  self.paused = presented;
}

- (void)didChangeViewerProfile {
  // Renderer's OnResume also refreshes viewer profile.
  _renderer->OnResume();
}

- (void)shouldDisableIdleTimer:(BOOL)shouldDisable {
  [UIApplication sharedApplication].idleTimerDisabled = shouldDisable;
}

@end
