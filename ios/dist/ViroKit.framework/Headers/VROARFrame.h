//
//  VROARFrame.h
//  ViroKit
//
//  Created by Raj Advani on 6/6/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROARFrame_h
#define VROARFrame_h

#include <memory>
#include <vector>
#include <set>

class VROARCamera;
class VROARAnchor;
class VROMatrix4f;
class VROVector3f;
class VROViewport;
class VROTextureSubstrate;
class VROARHitTestResult;
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
     Perform a hit test on the given point in the viewport. The coordinate 
     system is viewport pixels (e.g. the coordinate system in which
     VROViewport is defined).
     */
    virtual std::vector<VROARHitTestResult> hitTest(int x, int y, std::set<VROARHitTestResultType> types) = 0;

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
    virtual VROMatrix4f getViewportToCameraImageTransform() = 0;
    
    /*
     Return the estimated intensity of ambient light in the physical scene.
     */
    virtual float getAmbientLightIntensity() const = 0;
    
    /*
     Get all the anchors representing tracked positions and objects in the
     scene.
     */
    virtual const std::vector<std::shared_ptr<VROARAnchor>> &getAnchors() const = 0;
    
};

#endif /* VROARFrame_h */
