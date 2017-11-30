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

#import <GLKit/GLKit.h>

#import "GVRHeadPose.h"

/**
 * Defines a class that provides rendering of monoscopic and stereoscopic (VR)
 * content to display of the mobile device. The property ::VRModeEnabled controls
 * the display of monoscopic vs. stereoscopic content.
 *
 * This class is meant to be subclassed in order to implement application specific rendering.
 * Subclasses should override ::update:, ::draw and ::handleTrigger methods.
 *
 * Call ::initializeGl and ::clearGl before and after rendering on a valid GL context. Application
 * should call ::drawFrame: on every iteration of the render loop on a valid GL context. The
 * ::pause method should be called to pause head tracking and rendering.
 *
 * Although this class makes no assumption of threading context, it has been tested to be used
 * only on the main thread.
 *
 * Applications can manually add yaw and pitch rotation to the current head pose.
 */
@interface GVRRenderer : NSObject

/**
 * The vrModeEnabled property allows switching between monoscopic and stereoscopic (VR) rendering.
 * Stereoscopic rendering should be enabled only in fullscreen mode.
 */
@property(nonatomic) BOOL VRModeEnabled;


/** Initializes GL resources used by GVR. This should be called on a valid GL context. */
- (void)initializeGl;

/** Releases GL resources used by GVR. This should be called on a valid GL context. */
- (void)clearGl;

/**
 * Draws the monoscopic or stereoscopic frame depending upon VRModeEnabled property. Calls
 * ::draw:headPose method of subclasses to perform the actual drawing. This method should
 * be called on every iteration of the render loop.
 *
 * @param nextFrameTime The timestamp of when the frame will be rendered to the display.
 */
- (void)drawFrame:(NSTimeInterval)nextFrameTime;

/**
 * Updates the rendering based on currently paired viewer profile. Call this if the user
 * pairs a Cardboard viewer.
 */
- (void)refresh;

/**
 * Calls the method ::handleTriger: with the current head pose. Returns YES if
 * the subclasses handled this method.
 */
- (BOOL)handleTrigger;

/**
 * Add yaw and pitch rotation to the current head pose. This should be the delta
 * from the last update. Internally, this is cumulatively applied to the current
 * head pose. See ::resetHeadRotation to clear additional rotation.
 */
- (void)addToHeadRotationYaw:(CGFloat)yaw andPitch:(CGFloat)pitch;

/**
 * Clears any additional yaw and pitch rotation applied to the current head pose.
 * This method also recenters head tracking.
 */
- (void)resetHeadRotation;

/**
 * Call this when the render display size changes, including the current interface
 * orientation. When ::VRModeEnabled is set to YES, the size should be fullscreen
 * and orientation should be UIInterfaceOrientationLandscapeRight.
 */
- (void)setSize:(CGSize)size andOrientation:(UIInterfaceOrientation)orientation;

/**
 * Pauses or resumes head tracking and rendering. Subclasses should superclass's implementation.
 * @param pause YES to pause and NO to resume.
 */
- (void)pause:(BOOL)pause;

/**
 * Subclasses should override this method to prepare for drawing with the
 * supplied head pose. This is called for every frame, once per frame.
 */
- (void)update:(GVRHeadPose *)headPose;

/**
 * Subclasses should override this method to draw the content for the supplied
 * head pose. This would be called every frame, once for monoscopic and twice for
 * stereoscopic rendering. For stereoscopic rendering, the headpose will contain
 * eye transform matrix.
 */
- (void)draw:(GVRHeadPose *)headPose;

/**
 * Subclasses should override this method to handle user pressing the trigger in the
 * Cardboard viewer. They should return YES if the trigger action is handled.
 * @param headPose The headpose used to draw the view.
 */
- (BOOL)handleTrigger:(GVRHeadPose *)headPose;

@end
