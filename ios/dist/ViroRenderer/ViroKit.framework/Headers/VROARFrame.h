//
//  VROARFrame.h
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

#ifndef VROARFrame_h
#define VROARFrame_h

#include "VROARPointCloud.h"

#include <memory>
#include <vector>
#include <set>
#include "VROVector3f.h"

class VROARCamera;
class VROARAnchor;
class VROARHitTestResult;
class VROMatrix4f;
enum class VROARHitTestResultType;
enum class VROCameraOrientation;

/*
 The continual output of a VROARSession. These frames contain the current camera
 video image, camera parameters, and updated anchors.
 */
class VROARFrame {
public:
    
    VROARFrame() {}
    virtual ~VROARFrame() {}
    
    /*
     Get the timestamp, in seconds.
     */
    virtual double getTimestamp() const = 0;
    
    /*
     Contains information about the camera position, orientation, and imaging
     parameters for this frame.
     */
    virtual const std::shared_ptr<VROARCamera> &getCamera() const = 0;
    
    /*
     Get the orientation of this frame.
     */
    virtual VROCameraOrientation getOrientation() const = 0;
    
    /*
     Perform a hit test on the given point in the viewport. The coordinate 
     system is viewport pixels (e.g. the coordinate system in which
     VROViewport is defined).
     */
    virtual std::vector<std::shared_ptr<VROARHitTestResult>> hitTest(int x, int y, std::set<VROARHitTestResultType> types) = 0;

    /*
     Perform a hit test along the ray from origin to destination, in world coordinates
     */
    virtual std::vector<std::shared_ptr<VROARHitTestResult>> hitTestRay(VROVector3f *origin,
                                                                     VROVector3f *destination ,
                                                                     std::set<VROARHitTestResultType> types) = 0;
    /*
     Returns the affine transform to move from viewport space to camera
     image space. Camera image space is the texture coordinate space of
     the camera's image, ranging from (0,0) at the upper left to (1,1) on
     the lower right. Viewport space is the coordinate space of the current
     viewport, taking into account the current orientation.
     
     To render the camera image, either this transform should be applied to
     the camera background's texture coordinates, or the *inverse* of this
     transform should be applied to the camera background's vertices.
     This ensures the camera image maps correctly to the current viewport and
     orientation.
     */
    virtual VROMatrix4f getViewportToCameraImageTransform() const = 0;
    
    /*
     Return the estimated intensity of ambient light in the physical scene.
     */
    virtual float getAmbientLightIntensity() const = 0;

    /*
     Return the estimated color of ambient light in the physical scene, in linear RGB space.
     */
    virtual VROVector3f getAmbientLightColor() const = 0;

    /*
     Get all the anchors representing tracked positions and objects in the
     scene.
     */
    virtual const std::vector<std::shared_ptr<VROARAnchor>> &getAnchors() const = 0;

    /*
     Retrieves the point cloud from this frame.
     */
    virtual std::shared_ptr<VROARPointCloud> getPointCloud() = 0;
};

#endif /* VROARFrame_h */
