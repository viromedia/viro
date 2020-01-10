//
//  VROTransformDelegate
//  ViroRenderer
//
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

#ifndef VROTransformDelegate_h
#define VROTransformDelegate_h

#include <memory>
#include <vector>
#include "VROVector3f.h"
#include "VROTime.h"

/*
  Delegate that is attached to a node for notifying the bridge regarding transformation updates.
 */
class VROTransformDelegate {
public:
    VROTransformDelegate(double distanceFilter) {
        _distanceFilter = distanceFilter;
    }
    virtual ~VROTransformDelegate() {}
    virtual void onPositionUpdate(VROVector3f position)=0;

    /*
     Called from a native vroNode that this delegate is attached to and in turn notifies the bridge
     whenever a animation transaction occurs. Filtering is also performed to reduce the number of
     calls across the renderer-to-javascript bridge.
     */
    void processPositionUpdate(VROVector3f position, bool force) {
        double currentRenderTime = VROTimeCurrentMillis();
        if (_lastSampleTimeMs + 16 >= currentRenderTime && !force) {
            return;
        }

        if (_lastPositionUpdate.distance(position) < _distanceFilter && !force){
            return;
        }

        _lastSampleTimeMs = currentRenderTime;
        _lastPositionUpdate = position;
        onPositionUpdate(position);
    }

private:
    double _lastSampleTimeMs = 0;
    double _distanceFilter;
    VROVector3f _lastPositionUpdate;
};
#endif
