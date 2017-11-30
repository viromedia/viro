/**
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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-modular-include-in-framework-module"
#include "vr/gvr/capi/include/gvr.h"
#pragma clang diagnostic pop

/**
 * @enum GVREye
 *
 * Enumeration of the left and right eyes, used to identify the correct
 * rendering parameters needed for stereo rendering.
 */
typedef NS_ENUM(NSInteger, GVREye) {
  kGVRLeftEye = GVR_LEFT_EYE,   /**< Left eye. */
  kGVRRightEye = GVR_RIGHT_EYE, /**< Right eye. */
  kGVRCenterEye,                /**< Center eye. */
};

/**
 * Defines a struct to hold half field of view angles, in degrees.
 */
typedef struct {
  CGFloat left;
  CGFloat right;
  CGFloat bottom;
  CGFloat top;
} GVRFieldOfView;

/**
 * Defines a class that provides current head pose transforms.
 */
@interface GVRHeadPose : NSObject

/** The current |GVREye| eye being rendered with the head pose. */
@property(nonatomic, readonly) GVREye eye;

/** The head pose transform matrix. Includes manual yaw and pitch rotations. */
@property(nonatomic, readonly) GLKMatrix4 headTransform;

/** The eye transform relative to the head transform. For monoscopic this is identity matrix. */
@property(nonatomic, readonly) GLKMatrix4 eyeTransform;

/** The view (camera) transform that is the composite of head and eye transforms. */
@property(nonatomic, readonly) GLKMatrix4 viewTransform;

/** The projection transform computed from ::setProjectionMatrixWithNear:Far: */
@property(nonatomic, readonly) GLKMatrix4 projectionTransform;

/** The current display viewport. */
@property(nonatomic, readonly) CGRect viewport;

/** The current field of view. This depends on paired Cardboard viewer in stereoscopic mode. */
@property(nonatomic, readonly) GVRFieldOfView fieldOfView;

/** The timestamp when this frame will be presented. */
@property(nonatomic, readonly) NSTimeInterval nextFrameTime;

/**
 * Computes the projection matrix with near and far planes and is accessed from projectionTransform
 * property.
 */
- (void)setProjectionMatrixWithNear:(CGFloat)near far:(CGFloat)far;

/** Returns the projection matrix given near and far planes. */
- (GLKMatrix4)projectionMatrixWithNear:(CGFloat)near far:(CGFloat)far;

@end
