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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-modular-include-in-framework-module"
#import "GVROverlayView.h"
#pragma clang diagnostic pop
#import "GVRRenderer.h"

/**
 * Defines a view to handle GVR rendering. It provides its own render loop using |CADisplayLink|. It
 * can be initialized with an instance of |GVRRenderer|.
 */
@interface GVRRendererView : GLKView

/** Convenience initializer with the provided renderer. If another initializer is used, the
 * renderer is initialized to |GVRSceneRenderer}.
 *
 * @param renderer The renderer used to render the contents of the view.
 */
- (instancetype)initWithRenderer:(GVRRenderer *)renderer;

/** The renderer used to render the contents of the view. */
@property(nonatomic, readonly) GVRRenderer *renderer;

/** The overlay view used to provide a UI layer for Cardboard features such as icons to switch
 * between monoscopic and stereoscopic rendering, a transition view to prompt the user to insert
 * the phone is the viewer and to pair to a different viewer. */
@property(nonatomic, readonly) GVROverlayView* overlayView;

/** Allows toggling between monoscopic and stereoscopic (VR mode) rendering. */
@property(nonatomic) BOOL VRModeEnabled;

/** Pause or resume the |CADisplayLink| render loop. */
@property(nonatomic) BOOL paused;

@end
