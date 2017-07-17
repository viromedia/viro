//
//  VROTransformDelegate
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

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
