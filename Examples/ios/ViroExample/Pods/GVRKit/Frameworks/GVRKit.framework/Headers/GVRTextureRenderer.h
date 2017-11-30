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

#import "GVRSceneRenderer.h"

/** Defines mesh type needed for rendering stereoscopic content. */
typedef NS_ENUM(NSInteger, GVRMeshType) {
  kGVRMeshTypeStereoLeftRight = 0,
  kGVRMeshTypeStereoTopBottom,
  kGVRMeshTypeMonoscopic
};

/**
 * Defines a renderer for OpenGL textures. It supports rendering individual
 * image textures and dual (Y + UV) video textures. These textures can be
 * mapped to spherical or quad meshes.
 */
@interface GVRTextureRenderer : NSObject<GVRRenderObject>

/** The position transform, in world space, where the texture will be placed. */
@property(nonatomic) GLKMatrix4 position;

/** The minimum bounding box vertex position. */
@property(nonatomic, readonly) GLKVector3 aabbMin;

/** The maximum bounding box vertex position. */
@property(nonatomic, readonly) GLKVector3 aabbMax;

/**
 * Set spherical mesh for the texture. The mesh does not have be closed.
 *
 * @param radius Size of the sphere in meters. Must be > 0.
 * @param latitudes Number of rows that make up the sphere. Must be >= 1.
 * @param longitudes Number of columns that make up the sphere. Must be >= 1.
 * @param verticalFov Total latitudinal degrees that are covered by the sphere.
 *                    Must be in (0, 180].
 * @param horizontalFov Total longitudinal degrees that are covered by the
 *                      sphere.Must be in (0, 360].
 * @param meshType The mesh type to use depending upon the media format.
 */
- (void)setSphericalMeshOfRadius:(CGFloat)radius
                       latitudes:(NSInteger)latitudes
                      longitudes:(NSInteger)longitudes
                     verticalFov:(NSInteger)verticalFov
                   horizontalFov:(NSInteger)horizontalFov
                        meshType:(GVRMeshType)meshType;

/**
 * Set a quad mesh with the given width and height. The unit for width and
 * height is specified in meters.
 *
 * @param width The width in meters.
 * @param height The height in meters.
 * @param meshType The mesh type to use depending upon the media format.
 *
 * Use this to compute meters from pixels:
 * 1 meter width at 1 meter depth = 2 * atan(0.5) = 53.13 degrees per meter.
 * 15 pixels/degree * 53.13 degrees/meter = 796.951535313 pixels/meter.
 */
- (void)setQuadMeshOfWidth:(CGFloat)width
                    height:(CGFloat)height
                  meshType:(GVRMeshType)meshType;

/** Sets the texture id for an image. See |GVRImageRenderer|. */
- (void)setImageTextureId:(GLuint)textureId;

/**
 * Sets the textures for Y and UV of an individual video frame, along with the
 * color conversion matrix. See |GVRVideoRenderer|.
 */
- (void)setVideoYTextureId:(GLuint)yTextureId
               uvTextureId:(GLuint)uvTextureId
     colorConversionMatrix:(GLKMatrix4)colorConversionMatrix;

@end
