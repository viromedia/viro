//
//  VROEventDelegate.h
//  ViroRenderer
//
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROEventDelegate_h
#define VROEventDelegate_h

#include <stdio.h>
#include <vector>
#include <string>
#include <memory>
#include <set>
#include <map>

/**
 * Class for both registering for and implementing event delegate callbacks.
 */
class VROEventDelegate {
public:
    /**
     * Enum EventTypes that are supported by this delegate.
     *
     * IMPORTANT: Enum values should match EventType within EventDelegateJni.java
     * as the standard format to be passed through the JNI layer when enabling /
     * disabling delegate event callbacks. Do Not change the Enum Values!!!
     * Simply add additional event types as need be.
     */
    enum EventType {
        ON_TAP = 1,
        ON_GAZE = 2,
        ON_GAZE_DISTANCE = 3
    };

    VROEventDelegate(){
        // Disable all event callbacks by default
        _enabledEventMap[VROEventDelegate::EventType::ON_TAP] = false;
        _enabledEventMap[VROEventDelegate::EventType::ON_GAZE] = false;
        _enabledEventMap[VROEventDelegate::EventType::ON_GAZE_DISTANCE] = false;
    }

    /**
     * Informs the renderer to enable / disable the triggering of
     * specific EventType delegate callbacks.
     */
    void setEnabledEvent(VROEventDelegate::EventType type, bool enabled){
        _enabledEventMap[type] = enabled;
    }

    bool isEventEnabled(VROEventDelegate::EventType type){
        return _enabledEventMap[type];
    }

    /*
     * Delegate events triggered by the EventManager.
     */
    virtual void onTapped() {
        //No-op
    }
    virtual void onGaze(bool isGazing) {
        //No-op
    }
    virtual void onGazeHitDistance(float distance) {
        //No-op
    }

private:
    std::map<VROEventDelegate::EventType, bool> _enabledEventMap;
};
#endif
