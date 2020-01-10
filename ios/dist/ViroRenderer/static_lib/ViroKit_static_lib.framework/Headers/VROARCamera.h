//
//  VROARCamera.h
//  ViroKit
//
//  Created by Raj Advani on 6/6/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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

#ifndef VROARCamera_h
#define VROARCamera_h

#include <stdio.h>

class VROViewport;
class VROMatrix4f;
class VROVector3f;
class VROFieldOfView;

enum class VROARTrackingState {
    Unavailable = 1,
    Limited = 2,
    Normal = 3
};

/*
 Provides reasons for why we are in Limited tracking state.
 */
enum class VROARTrackingStateReason {
    None = 1,
    ExcessiveMotion = 2,
    InsufficientFeatures = 3,
    InsufficientLight = 4,
};

/*
 Contains information about the current camera position and imaging
 characteristics. Updated periodically by the VROARSession.
 */
class VROARCamera {
public:
    
    VROARCamera() {}
    virtual ~VROARCamera() {}
    
    /*
     Get the quality of the tracking. If we are in limited tracking state,
     getLimitedTrackingStateReason() will return the reason for this state.
     */
    virtual VROARTrackingState getTrackingState() const = 0;
    virtual VROARTrackingStateReason getLimitedTrackingStateReason() const = 0;

    /*
     Get the position and rotation of the user in world space. These values
     are derived from the AR camera's extrinsic matrix, acquired via visual and
     inertial algorithms. They can be used to construct the view matrix for the
     renderer.
     */
    virtual VROMatrix4f getRotation() const = 0;
    virtual VROVector3f getPosition() const = 0;
    
    /*
     Get the projection matrix needed to render a 3D scene to match the image
     captured by this AR camera. The returned matrix is a concatenation of the
     AR camera's intrinsic matrix (3D space --> 2D space) and the renderer's
     orthographic projection matrix (2D space --> normalized device space).
     This function should also output the field of view in the provided output
     variable.
     */
    virtual VROMatrix4f getProjection(VROViewport viewport, float near, float far, VROFieldOfView *outFOV) = 0;
    
    /*
     Get the image size (width, height) for the camera. Stored in the
     vector's x and y components.
     */
    virtual VROVector3f getImageSize() = 0;
    
};

#endif /* VROARCamera_h */
