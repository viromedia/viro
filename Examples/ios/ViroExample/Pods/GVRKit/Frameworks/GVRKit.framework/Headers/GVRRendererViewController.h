/*
 * Copyright 2017 Google Inc. All Rights Reserved.
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

#import "GVRRendererView.h"

/**
 * @enum GVRDisplayMode
 *
 * Enumeration of the display modes, used to identify the |GVRRenderer| used for rendering.
 */
typedef NS_ENUM(NSInteger, GVRDisplayMode) {
  kGVRDisplayModeEmbedded,
  kGVRDisplayModeFullscreenMonoscopic,
  kGVRDisplayModeFullscreenStereoscopic,
};

/** Defines a delegate used to provide an instance of |GVRRenderer| to the controller's view. */
@protocol GVRRendererViewControllerDelegate <NSObject>

- (GVRRenderer *)rendererForDisplayMode:(GVRDisplayMode)displayMode;

@optional

/** Handle user tapping the renderer view. */
- (void)didTapTriggerButton;

/** Return YES to hide the transition view. */
- (BOOL)shouldHideTransitionView;

@end

/**
 * Defines a view controller to host a |GVRRendererView|. This should be used for presenting
 * fullscreen. It also handles interface orientation logic and interfacing with the overlay view
 * during viewer pairing. This view controller is also recommended when embedding its view in
 * another view controller's view. In that case, this view controller should be added as a child
 * view controller to handle |viewWillAppear| and |viewWillDisappear| lifecycle events.
 */
@interface GVRRendererViewController : UIViewController

/** A convenience initializer to be used with the supplied |GVRRenderer|. */
- (instancetype)initWithRenderer:(GVRRenderer *)renderer;

@property(nonatomic, weak) id<GVRRendererViewControllerDelegate> delegate;

@property(nonatomic, readonly) GVRRendererView *rendererView;

@end
