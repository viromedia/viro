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

#import "GVRRenderer.h"

/**
 * Defines an interface for a render object that is placed in a scene graph.
 */
@protocol GVRRenderObject<NSObject>

/**
 * Returns YES if the render object is initialized. This is useful when the
 * object is moved around in the scene graph.
 */
@property(nonatomic, readonly) BOOL initialized;

/** Allows hiding or showing the render object in the scene. */
@property(nonatomic) BOOL hidden;

/** Initializes GL resources needed for rendering the render object. */
- (void)initializeGl;

/** Renders the object given the head pose. */
- (void)draw:(GVRHeadPose *)headPose;

@optional
/** Releases the GL resources. */
- (void)clearGl;

/** Updates GL state needed before ::draw: is called. */
- (void)update:(GVRHeadPose *)headPose;

/** Pauses or resumes rendering. */
- (void)pause:(BOOL)pause;

/**
 * Returns YES if render object handles the trigger button press. This stops
 * render objects next in the display list from handling the trigger press.
 */
- (BOOL)handleTrigger:(GVRHeadPose *)headPose;
@end


/**
 * Defines a simple list of render objects. Each item in the list implements the
 * |GVRRenderObject| protocol, including GVRRenderList itself. This results in
 * a scene graph data structure. Objects in the list are drawn in depth-first order.
 */
@interface GVRRenderList : NSObject<GVRRenderObject>
- (void)addRenderObject:(id<GVRRenderObject>)renderTarget;
- (void)insertRenderObject:(id<GVRRenderObject>)renderTarget atIndex:(NSUInteger)index;
- (void)removeRenderObject:(id<GVRRenderObject>)renderTarget;
- (void)removeRenderObjectAtIndex:(NSUInteger)index;
- (void)removeAll;
- (id<GVRRenderObject>)objectAtIndex:(NSUInteger)index;
- (NSUInteger)count;
@end

/** Defines a scene renderer with a render list of |GVRRenderObject| objects. */
@interface GVRSceneRenderer : GVRRenderer

@property(nonatomic) GVRRenderList *renderList;

@property(nonatomic) BOOL hidesReticle;

@end

// Helper methods used during OpenGL rendering.
void checkGLError(const char *label);
GLuint loadShader(GLenum type, const char *shader_src);
bool checkProgramLinkStatus(GLuint shader_program);

