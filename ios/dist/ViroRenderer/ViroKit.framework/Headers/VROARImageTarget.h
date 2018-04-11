//
//  VROARImageTarget.h
//  ViroKit
//
//  Created by Andy Chu on 1/30/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROARImageTarget_h
#define VROARImageTarget_h

#include <memory>
#include "VROARSession.h"

#if ENABLE_OPENCV
#include <opencv2/core/mat.hpp>
#endif /* ENABLE_OPENCV */

/*
 The orientation of the given target image.
 */
enum class VROImageOrientation {
    Up,
    Down,
    Left,
    Right
};

class VROARImageTarget {
public:
    
    VROARImageTarget(VROImageOrientation orientation, float physicalWidth) :
        _orientation(orientation),
        _physicalWidth(physicalWidth) {}

    virtual ~VROARImageTarget() {}

    /*
     We need this function because the creation of the ImageTarget may
     not know what the tracking impl type is. This is called by the
     ARSession/ARTrackingSession.
     */
    virtual void initWithTrackingImpl(VROImageTrackingImpl impl) {};

    void setAnchor(std::shared_ptr<VROARAnchor> anchor) {
        _anchor = anchor;
    }

    std::shared_ptr<VROARAnchor> getAnchor() {
        return _anchor.lock();
    }

    VROImageOrientation getOrientation() {
        return _orientation;
    }

    float getPhysicalWidth() {
        return _physicalWidth;
    }

#if ENABLE_OPENCV
    cv::Mat getTargetMat() {
        return _targetMat;
    }

    void setTargetMat(cv::Mat targetMat) {
        _targetMat = targetMat;
    }
#endif /* ENABLE_OPENCV */

private:
    std::weak_ptr<VROARAnchor> _anchor;
    VROImageOrientation _orientation;
    float _physicalWidth;


#if ENABLE_OPENCV
    cv::Mat _targetMat;
#endif /* ENABLE_OPENCV */
};

#endif /* VROARImageTarget_h */
