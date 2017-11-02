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
#include <cstdint>

class VROARCamera;
class VROARAnchor;
class VROMatrix4f;
class VROVector4f;
class VROViewport;
class VROTextureSubstrate;
class VROARHitTestResult;
enum class VROARHitTestResultType;
enum class VROCameraOrientation;

// TODO: possible need to guard uint64_t usage with #ifdef UINT64_MAX, seems fine on iOS/Android
class VROARPointCloud {
public:
    VROARPointCloud() {}
    VROARPointCloud(std::vector<VROVector4f> points, std::vector<uint64_t> identifiers) :
        _points(points),
        _identifiers(identifiers) {}

    ~VROARPointCloud() {}

    /*
     Retrieves the point that make up this point cloud. Note: the 4th value in the
     vector is a "confidence" value only available on Android.
     */
    std::vector<VROVector4f> getPoints() {
        return _points;
    }

    /*
     Retrieves the identifiers corresponding to each point. Note: iOS only (it's empty
     on Android).
     */
    std::vector<uint64_t> getIdentifiers() {
        return _identifiers;
    }

private:
    std::vector<VROVector4f> _points;
    std::vector<uint64_t> _identifiers; // used only on iOS (Android does not provide identifiers).
};

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
     Return the estimate color temperature of ambient light in the physical scene.
     */
    virtual float getAmbientLightColorTemperature() const = 0;

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
