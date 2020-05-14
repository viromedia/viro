//
//  VROARImageTarget.h
//  ViroKit
//
//  Created by Andy Chu on 1/30/18.
//  Copyright © 2018 Viro Media. All rights reserved.
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

#ifndef VROARImageTarget_h
#define VROARImageTarget_h

#include <memory>
#include "VROARSession.h"

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

private:
    std::weak_ptr<VROARAnchor> _anchor;
    VROImageOrientation _orientation;
    float _physicalWidth;
};

#endif /* VROARImageTarget_h */
