//
//  ViroKit.h
//  ViroKit
//
//  Created by Raj Advani on 12/9/15.
//  Copyright © 2015 Viro Media. All rights reserved.
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

#import <UIKit/UIKit.h>

//! Project version number for ViroKit.
FOUNDATION_EXPORT double ViroKitVersionNumber;

//! Project version string for ViroKit.
FOUNDATION_EXPORT const unsigned char ViroKitVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <ViroKit/PublicHeader.h>

#import <ViroKit/VRODefines.h>
#import <ViroKit/VROOpenGL.h>
#import <ViroKit/VROSceneController.h>
#import <ViroKit/VROView.h>
#import <ViroKit/VROViewMetal.h>
#import <ViroKit/VROViewControllerGVR.h>
#import <ViroKit/VROViewGVR.h>
#import <ViroKit/VROViewScene.h>
#import <ViroKit/VRORendererConfiguration.h>
#import <ViroKit/VRORenderDelegate.h>
#import <ViroKit/VRORenderContext.h>
#import <ViroKit/VRODriver.h>
#import <ViroKit/VRORenderParameters.h>
#import <ViroKit/VROFrameListener.h>
#import <ViroKit/VROFrameTimer.h>
#import <ViroKit/VROFrameScheduler.h>
#import <ViroKit/VROFrameSynchronizer.h>

// Model Loader
#import <ViroKit/VROOBJLoader.h>
#import <ViroKit/VROFBXLoader.h>
#import <ViroKit/VROGLTFLoader.h>
#import <ViroKit/VROHDRLoader.h>

// Core Scene Graph
#import <ViroKit/VROScene.h>
#import <ViroKit/VROSceneDelegateiOS.h>
#import <ViroKit/VROCamera.h>
#import <ViroKit/VRONodeCamera.h>
#import <ViroKit/VROEventDelegateiOS.h>
#import <ViroKit/VRONode.h>
#import <ViroKit/VROPortal.h>
#import <ViroKit/VROPortalFrame.h>
#import <ViroKit/VROPortalDelegateiOS.h>
#import <ViroKit/VROGeometry.h>
#import <ViroKit/VROGeometryElement.h>
#import <ViroKit/VROGeometrySource.h>
#import <ViroKit/VROMaterial.h>
#import <ViroKit/VROMaterialVisual.h>
#import <ViroKit/VROAnimatedTextureOpenGL.h>
#import <ViroKit/VROTexture.h>
#import <ViroKit/VROLight.h>
#import <ViroKit/VROImage.h>
#import <ViroKit/VROImageiOS.h>
#import <ViroKit/VROShaderModifier.h>
#import <ViroKit/VROShaderProgram.h>
#import <ViroKit/VROTransaction.h>
#import <ViroKit/VROHitTestResult.h>
#import <ViroKit/VROConstraint.h>
#import <ViroKit/VROBillboardConstraint.h>
#import <ViroKit/VROTransformConstraint.h>
#import <ViroKit/VROTransformDelegate.h>
#import <ViroKit/VROTransformDelegateiOS.h>
#import <ViroKit/VROTree.h>
#import <ViroKit/VROParticleEmitter.h>
#import <ViroKit/VROParticle.h>
#import <ViroKit/VROParticleModifier.h>

// PostProcess
#import <ViroKit/VROChoreographer.h>
#import <ViroKit/VRORenderPass.h>
#import <ViroKit/VROGaussianBlurRenderPass.h>

// Animation
#import <ViroKit/VROAnimation.h>
#import <ViroKit/VROAnimatable.h>
#import <ViroKit/VROPropertyAnimation.h>
#import <ViroKit/VROMaterialAnimation.h>
#import <ViroKit/VROExecutableAnimation.h>
#import <ViroKit/VROAnimationGroup.h>
#import <ViroKit/VROAnimationChain.h>
#import <ViroKit/VROTimingFunction.h>
#import <ViroKit/VROTimingFunctionBounce.h>
#import <ViroKit/VROTimingFunctionCubicBezier.h>
#import <ViroKit/VROTimingFunctionEaseInEaseOut.h>
#import <ViroKit/VROTimingFunctionEaseIn.h>
#import <ViroKit/VROTimingFunctionEaseOut.h>
#import <ViroKit/VROTimingFunctionLinear.h>
#import <ViroKit/VROTimingFunctionPowerDeceleration.h>
#import <ViroKit/VROAction.h>
#import <ViroKit/VROLazyMaterial.h>
#import <ViroKit/VROMorpher.h>

// UI
#import <ViroKit/VROReticle.h>
#import <ViroKit/VROText.h>
#import <ViroKit/VROTypeface.h>
#import <ViroKit/VROTypefaceCollection.h>

// Video
#import <ViroKit/VROVideoSurface.h>
#import <ViroKit/VROVideoTexture.h>
#import <ViroKit/VROVideoTextureiOS.h>
#import <ViroKit/VROVideoDelegateiOS.h>
#import <ViroKit/VROCameraTexture.h>
#import <ViroKit/VROCameraTextureiOS.h>

// Audio
#import <ViroKit/VROAudioPlayer.h>
#import <ViroKit/VROAudioPlayeriOS.h>
#import <ViroKit/VROSound.h>
#import <ViroKit/VROSoundGVR.h>
#import <ViroKit/VROSoundData.h>
#import <ViroKit/VROSoundDataGVR.h>
#import <ViroKit/VROSoundDataDelegate.h>
#import <ViroKit/VROSoundDelegate.h>
#import <ViroKit/VROSoundDelegateiOS.h>

// Math
#import <ViroKit/VROQuaternion.h>
#import <ViroKit/VROPlane.h>
#import <ViroKit/VROFrustum.h>
#import <ViroKit/VROFrustumPlane.h>
#import <ViroKit/VROFrustumBoxIntersectionMetadata.h>
#import <ViroKit/VROBoundingBox.h>
#import <ViroKit/VROVector3f.h>
#import <ViroKit/VROVector4f.h>
#import <ViroKit/VROMatrix4f.h>
#import <ViroKit/VROMath.h>
#import <ViroKit/VROTriangle.h>

// Shapes
#import <ViroKit/VROBox.h>
#import <ViroKit/VROSphere.h>
#import <ViroKit/VROSurface.h>
#import <ViroKit/VROPolygon.h>
#import <ViroKit/VROPolyline.h>
#import <ViroKit/VROTorusKnot.h>
#import <ViroKit/VROShapeUtils.h>

// Controller
#import <ViroKit/VROInputControllerBase.h>
#import <ViroKit/VROInputControllerCardboardiOS.h>
#import <ViroKit/VROInputPresenterCardboardiOS.h>
#import <ViroKit/VROBodyTrackerController.h>

// Util
#import <ViroKit/VROTime.h>
#import <ViroKit/VROLog.h>
#import <ViroKit/VROByteBuffer.h>
#import <ViroKit/VROImageUtil.h>
#import <ViroKit/VROData.h>
#import <ViroKit/VROGeometryUtil.h>
#import <ViroKit/VROTextureUtil.h>
#import <ViroKit/VROTaskQueue.h>
#import <ViroKit/VRODeviceUtil.h>

// Physics
#import <ViroKit/VROPhysicsShape.h>
#import <ViroKit/VROPhysicsBody.h>
#import <ViroKit/VROPhysicsWorld.h>
#import <ViroKit/VROPhysicsBodyDelegate.h>
#import <ViroKit/VROPhysicsBodyDelegateiOS.h>

// AR
#import <ViroKit/VROViewAR.h>
#import <ViroKit/VROARSession.h>
#import <ViroKit/VROARSessioniOS.h>
#import <ViroKit/VROARFrame.h>
#import <ViroKit/VROARFrameiOS.h>
#import <ViroKit/VROARFrameInertial.h>
#import <ViroKit/VROARCamera.h>
#import <ViroKit/VROARAnchor.h>
#import <ViroKit/VROARPlaneAnchor.h>
#import <ViroKit/VROARHitTestResult.h>
#import <ViroKit/VROARScene.h>
#import <ViroKit/VROARSceneController.h>
#import <ViroKit/VROARDeclarativeImageNode.h>
#import <ViroKit/VROARDeclarativeObjectNode.h>
#import <ViroKit/VROARDeclarativePlane.h>
#import <ViroKit/VROARDeclarativeNode.h>
#import <ViroKit/VROARNodeDelegateiOS.h>
#import <ViroKit/VROARSceneDelegateiOS.h>
#import <ViroKit/VROARShadow.h>
#import <ViroKit/VROARDeclarativeSession.h>
#import <ViroKit/VROARImageTarget.h>
#import <ViroKit/VROARImageTargetiOS.h>
#import <ViroKit/VROARObjectTarget.h>
#import <ViroKit/VROARObjectTargetiOS.h>

// Test
#import <ViroKit/VRORendererTest.h>
#import <ViroKit/VRORendererTestHarness.h>

// Body Tracking
#import <ViroKit/VROBodyPlayeriOS.h>
#import <ViroKit/VROBodyTrackeriOS.h>
#import <ViroKit/VROBodyTrackerYolo.h>
#import <ViroKit/VROBodyAnimData.h>
#import <ViroKit/VROPoseFilter.h>
#import <ViroKit/VROPoseFilterEuro.h>

// OpenCV
//#import <ViroKit/VROOpenCV.h>


