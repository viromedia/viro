//
//  VRORenderer.h
//  ViroRenderer
//
//  Created by Raj Advani on 4/5/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef VRORenderer_h
#define VRORenderer_h

#include <memory>
#include <vector>
#include "VROAtomic.h"
#include "VROSceneController.h"
#include "VROVector3f.h"
#include "VROQuaternion.h"
#include "VROMatrix4f.h"
#include "VROViewport.h"
#include "VROFieldOfView.h"
#include "VROFrameSynchronizer.h"
#include "VROInputControllerBase.h"
#include "VROPostProcessEffectFactory.h"
#include "VRORendererConfiguration.h"

class VROEye;
class VRONode;
class VRODriver;
class VRODebugHUD;
class VRONodeCamera;
class VROTimingFunction;
class VRORenderContext;
class VROFrameListener;
class VRORenderDelegateInternal;
class VROFrameScheduler;
class VROChoreographer;
class VRORenderMetadata;
enum class VROCameraRotationType;
enum class VROEyeType;
enum class VROTimingFunctionType;

// The (fixed) near clipping plane
static const float kZNear = 0.01;

// The far clipping plane to use if no scene is loaded; also serves as a
// mininum far clipping plane
static const float kZFar  = 50;

// Multiplified by the furthest out object in the active scene to determine
// the FCP.
static const float kZFarMultiplier = 1.15;

// Number of samples to collect when computing FPS
static const int kFPSMaxSamples = 100;

// The FOV we use for the larger dimension of the viewport, when in
// mono-rendering mode. This is similar to Hor+ scaling, in that one
// dimension is fixed, and other is dependent on the viewport. Note
// this value is degrees from edge to edge of the frustum.
static const double kFovMonoMajor = 90;

class VRORenderer {
public:

    VRORenderer(VRORendererConfiguration config, std::shared_ptr<VROInputControllerBase> inputController);
    virtual ~VRORenderer();

    /*
     Set the node that contains the camera through which we will view the
     scene.
     */
    void setPointOfView(std::shared_ptr<VRONode> node);

    /*
     Set the delegate that can be used to respond to renderer state changes.
     */
    void setDelegate(std::shared_ptr<VRORenderDelegateInternal> delegate);

    /*
     Set to true to enable the debug HUD, which draws frame data directly
     to the screen.
     */
    void setDebugHUDEnabled(bool enabled);

    /*
     Set renderer configuration properties. These are forwarded to the
     choreographer once it's created.
     */
    bool setHDREnabled(bool enableHDR);
    bool setPBREnabled(bool enablePBR);
    bool setShadowsEnabled(bool enableShadows);
    bool setBloomEnabled(bool enableBloom);

    /*
     Get the VROChoreographer, which can be used to customize the rendering
     technique.
     */
    const std::shared_ptr<VROChoreographer> getChoreographer() const;

#pragma mark - Viewport and FOV

    /*
     Get the field of view (vertical and horizontal) to use to render a viewport
     of the given size, given the FOV on the major or minor axis of the given degrees.
     The major axis is the axis with the larger screen width (e.g. X in landscape mode,
     Y in portrait mode). This enables users to simply specify the FOV for (e.g.) the major axis,
     and have that FOV maintain even after the device is reoriented and the major/minor
     axes switch.

     Note the given FOV values are degrees from edge to edge of the frustum.
     */
    static VROFieldOfView computeFOVFromMinorAxis(float minorAxisFOVDegrees, int viewportWidth,
                                                  int viewportHeight);
    static VROFieldOfView computeFOVFromMajorAxis(float majorAxisFOVDegrees, int viewportWidth,
                                                  int viewportHeight);

    /*
     Get the far clipping plane, as computed during the last prepareFrame().
     */
    float getFarClippingPlane() const;

    /*
     Compute the user-set field of view from the active VRONodeCamera and the viewport. If no
     field of view is set by the user, then returns the default monocular field of view.
     */
    VROFieldOfView computeUserFieldOfView(float viewportWidth, float viewportHeight) const;

    /*
     Get the field of view that was used to render the last frame. On AR and VR platforms this
     is typically set by the device. On other platforms it can be set by the user through the
     VRONodeCamera. This returns the FOV across the major (larger) axis.
     */
    float getActiveFieldOfView() const;

    /*
     Returns the axis across which the field of view applies. This is the major axis (the axis
     with the larger dimension).
     */
    VROFieldOfViewAxis getActiveFieldOfViewAxis() const;

    /*
     Get the camera look-at matrix, which is computed each frame after prepareFrame
     is invoked. This takes into account the headRotation provided in prepareFrame and
     the specified pointOfView.
     */
    VROMatrix4f getLookAtMatrix() const;
    
    /*
     Convert the given 3D point in the world into its corresponding 2D point on the screen.
     Note that each 2D point on the view corresponds to a continuous segment of points in
     the world, each at a different depth between the near and far clipping planes. The Z
     coordinate in the returned point identifies the depth of the projected point, where 0.0
     coincides with the near clipping plane, 1.0 coincides with the far clipping plane, and
     all other values are linearly interpolated between the two.
     */
    VROVector3f projectPoint(VROVector3f point);
    
    /*
     Convert the given 3D point in the world into its corresponding 2D point on the screen.
     Note that each 2D point on the view corresponds to a continuous segment of 3D points in
     the world, each at a different depth between the near and far clipping planes. The Z
     coordinate in the returned point identifies the depth of the projected point, where 0.0
     coincides with the near clipping plane, 1.0 coincides with the far clipping plane, and
     all other values are linear interpolations between the two.
     */
    VROVector3f unprojectPoint(VROVector3f point);

#pragma mark - Scene Controllers

    void setSceneController(std::shared_ptr<VROSceneController> sceneController,
                            std::shared_ptr<VRODriver> driver);

    void setSceneController(std::shared_ptr<VROSceneController> sceneController, float seconds,
                            VROTimingFunctionType timingFunctionType,
                            std::shared_ptr<VRODriver> driver);

    /*
     Applies scene specific post processing configuration effects to the renderer if we
     haven't yet done so already (since the scene had appeared), or if it had changed.
     */
    void updateSceneEffects(std::shared_ptr<VRODriver> driver, std::shared_ptr<VROScene> scene);
    
#pragma mark - Render Loop
    
    /*
     The metadata produced during the last prepareFrame.
     */
    std::shared_ptr<VRORenderMetadata> _renderMetadata;

    /*
     Prepare to render the next frame. This computes transforms and physics, processes
     animations, updates visibility, and sorts visible objects so they can be rendered
     swiftly for each eye.
     
     Note the projection matrix used here is used to construct the visibility frustum,
     not for rendering.
     */
    void prepareFrame(int frame, VROViewport viewport, VROFieldOfView fov,
                      VROMatrix4f headRotation, VROMatrix4f projection, std::shared_ptr<VRODriver> driver);
    
    /*
     Render the designated eye. Note how the eyeView matrix differs from headRotation in
     prepareFrame: eyeView is typically the *inverse* of the headRotation, translated by the
     interpupillary distance for the given eye.
     */
    void renderEye(VROEyeType eye, VROMatrix4f eyeView, VROMatrix4f eyeProjection,
                   VROViewport viewport, std::shared_ptr<VRODriver> driver);

    /*
     Render the HUD for the eye. The HUD follows the view, but is not 2D in that HUD elements
     can appear at different depths. The eyeFromHeadMatrix and eyeProjection are required for
     this simulation.
     */
    void renderHUD(VROEyeType eye, VROMatrix4f eyeFromHeadMatrix, VROMatrix4f eyeProjection,
                   std::shared_ptr<VRODriver> driver);

    /*
     Performs end-frame cleanup.
     */
    void endFrame(std::shared_ptr<VRODriver> driver);

#pragma mark - Integration
    
    std::shared_ptr<VROFrameSynchronizer> getFrameSynchronizer() {
        return _frameSynchronizer;
    }

    std::shared_ptr<VROInputControllerBase> getInputController(){
        return _inputController;
    }

    void setClearColor(VROVector4f color, std::shared_ptr<VRODriver> driver);
    
#pragma mark - Camera
   
    /*
     Get the camera used in the last frame.
     */
    const VROCamera &getCamera() const {
        return _context->getCamera();
    }
    
    /*
     Returns the last computed camera position.
     TODO: Revisit unifying Camera APIs in VIRO-2235.
     */
    VROVector3f getCameraPositionRealTime() const {
        return _lastComputedCameraPosition;
    }
    
    /*
     Returns the last computed camera euler rotation.
     TODO: Revisit unifying Camera APIs in VIRO-2235.
     */
    VROVector3f getCameraRotationRealTime() const {
        return _lastComputedCameraRotation;
    }
    
    /*
     Returns the last computed camera forward.
     TODO: Revisit unifying Camera APIs in VIRO-2235.
     */
    VROVector3f getCameraForwardRealTime() const {
        return _lastComputedCameraForward;
    }
    
    /*
     Set a delegate to receive camera movent events.
     TODO: Revisit unifying Camera APIs in VIRO-2235.
     */
    void setCameraDelegate(std::shared_ptr<VROCameraDelegate> delegate) {
        _cameraDelegate = delegate;
    }

    void setInputController(std::shared_ptr<VROInputControllerBase> inputController) {
        _inputController = inputController;
    }

    void performSceneHitTestWithPoint(int x, int y);
    
#pragma mark - VR Framework Specific
    
    // Some VR frameworks provide controls to allow the user to exit VR
    void requestExitVR();

private:

    bool _rendererInitialized;
    
    /*
     Maintains parameters used for scene rendering.
     */
    std::shared_ptr<VRORenderContext> _context;
    
    /*
     Sequences the render passes and targets. Defines the overall rendering
     technique.
     */
    std::shared_ptr<VROChoreographer> _choreographer;

    /*
     Controller used for handling all input events.
     */
    std::shared_ptr<VROInputControllerBase> _inputController;
    
    /*
     Delegate receiving scene rendering updates.
     */
    std::weak_ptr<VRORenderDelegateInternal> _delegate;
    
    /*
     HUD for displaying debug information.
     */
    std::unique_ptr<VRODebugHUD> _debugHUD;
    
    /*
     The initial configuration to use for the renderer. These settings can be
     changed directly in the VROChoreographer after the renderer is initialized.
     */
    VRORendererConfiguration _initialRendererConfig;
    
    /*
     Clear color used by the renderer. This is passed to the VROChoreographer
     after initialization.
     */
    VROVector4f _clearColor;

    /*
     Invoked on the rendering thread to perform thread-specific initialization.
     */
    void initRenderer(std::shared_ptr<VRODriver> driver);
  
#pragma mark - [Private] Camera and Visibility
    
    /*
     The node that owns the VRONodeCamera that will determine the point of
     view from which we display the scene.
     */
    std::shared_ptr<VRONode> _pointOfView;

    VROCamera updateCamera(const VROViewport &viewport, const VROFieldOfView &fov,
                           const VROMatrix4f &headRotation, const VROMatrix4f &projection);

    /*
     TODO: Revisit unifying Camera APIs in VIRO-2235.
     */
    VROAtomic<VROVector3f> _lastComputedCameraPosition;
    VROAtomic<VROVector3f> _lastComputedCameraRotation;
    VROAtomic<VROVector3f> _lastComputedCameraForward;
    std::shared_ptr<VROCameraDelegate> _cameraDelegate;

    VROMatrix4f _lastLeftEyeView;
    VROMatrix4f _lastRightEyeView;

#pragma mark - [Private] FPS Computation
    
    /*
     Variables for FPS computation. Array of samples taken, index of
     next sample, and sum of samples so far.
     */
    int _fpsTickIndex = 0;
    uint64_t _fpsTickSum = 0;
    uint64_t _fpsTickArray[kFPSMaxSamples];
    
    /*
     FPS is measured in ticks, which are the number of nanoseconds
     between consecurive calls to prepareFrame().
     */
    uint64_t _nanosecondsLastFrame;
    
    /*
     FPS is computed via moving average. Updates the moving average
     with the last tick.
     */
    void updateFPS(uint64_t newTick);
    double getFPS() const;
    
#pragma mark - [Private] Process scheduling
    
    /*
     Milliseconds per frame target; derived from desired FPS.
     */
    double _mpfTarget;
    
    /*
     The time the current frame started and ended its display functions.
     */
    double _frameStartTime;
    double _frameEndTime;
    
    /*
     Schedules tasks to be run on the rendering thread when remaining
     frame time allows.
     */
    std::shared_ptr<VROFrameScheduler> _frameScheduler;

#pragma mark - [Private] Scene and Scene Transitions
    
    std::shared_ptr<VROSceneController> _sceneController;
    std::shared_ptr<VROSceneController> _outgoingSceneController;
    bool _hasIncomingSceneTransition;

#pragma mark - [Private] Frame Listeners
    
    /*
     Manages per-frame listeners.
     */
    std::shared_ptr<VROFrameSynchronizer> _frameSynchronizer;
    
    void notifyFrameStart();
    void notifyFrameEnd();
};

#endif /* VRORenderer_h */
