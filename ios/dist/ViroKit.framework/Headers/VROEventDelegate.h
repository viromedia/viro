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
#include "VROVector3f.h"
#include "VROHitTestResult.h"
#include <limits>

static const float kOnFuseReset = std::numeric_limits<float>::max();

/**
 * Class for both registering for and implementing event delegate callbacks.
 */
class VROEventDelegate {
public:
    /**
     * Enum EventAction types that are supported by this delegate, used for
     * describing InputSources from InputTypes.h. For example, an OnClick
     * action may originate from a ViroDayDream AppButton inputSource.
     *
     * IMPORTANT: Enum values should match EventAction within EventDelegateJni.java
     * as the standard format to be passed through the JNI layer.
     * Do Not change the Enum Values!!! Simply add additional event types as need be.
     */
    enum EventAction{
        OnHover = 1,
        OnClick = 2,
        OnTouch = 3,
        OnMove = 4,
        OnControllerStatus = 5,
        OnSwipe = 6,
        OnScroll = 7,
        OnDrag = 8,
        OnFuse = 9
    };

    /**
     * ClickState enum describing the OnClick Event action.
     */
    enum ClickState {
        ClickDown = 1,
        ClickUp = 2,
        Clicked = 3
    };

    /**
     * TouchState enum describing the OnTouch Event action.
     */
    enum TouchState{
        TouchDown = 1,
        TouchDownMove = 2,
        TouchUp = 3,
    };

    enum SwipeState{
        SwipeUp = 1,
        SwipeDown = 2,
        SwipeLeft = 3,
        SwipeRight = 4
    };

    /**
     * Enum ControllerStatus types describing the availability status of the
     * current input controller.
     *
     * IMPORTANT: Enum values should match EventSource within EventDelegateJni.java
     * as the standard format to be passed through the JNI layer.
     * Do Not change the Enum Values!!! Simply add additional event types as need be.
     */
    enum ControllerStatus{
        Unknown = 1,
        Connecting = 2,
        Connected = 3,
        Disconnected = 4,
        Error = 5
    };

    // Disable all event callbacks by default
    VROEventDelegate(){
        _enabledEventMap[VROEventDelegate::EventAction::OnHover] = false;
        _enabledEventMap[VROEventDelegate::EventAction::OnClick] = false;
        _enabledEventMap[VROEventDelegate::EventAction::OnTouch] = false;
        _enabledEventMap[VROEventDelegate::EventAction::OnMove] = false;
        _enabledEventMap[VROEventDelegate::EventAction::OnControllerStatus] = false;
        _enabledEventMap[VROEventDelegate::EventAction::OnSwipe] = false;
        _enabledEventMap[VROEventDelegate::EventAction::OnScroll] = false;
        _enabledEventMap[VROEventDelegate::EventAction::OnDrag] = false;
        _enabledEventMap[VROEventDelegate::EventAction::OnFuse] = false;
    }

    /**
     * Informs the renderer to enable / disable the triggering of
     * specific EventSource delegate callbacks.
     */
    void setEnabledEvent(VROEventDelegate::EventAction type, bool enabled){
        _enabledEventMap[type] = enabled;
    }

    bool isEventEnabled(VROEventDelegate::EventAction type){
        return _enabledEventMap[type];
    }

    /*
     * Delegate events triggered by the VROInputControllerBase.
     */
    virtual void onHover(int source, bool isHovering) {
        //No-op
    }

    virtual void onClick(int source, ClickState clickState) {
        //No-op
    }

    virtual void onTouch(int source, TouchState touchState, float x, float y){
        //No-op
    }

    virtual void onMove(int source, VROVector3f rotation, VROVector3f position) {
        //No-op
    }

    virtual void onControllerStatus(int source, ControllerStatus status) {
        //No-op
    }

    virtual void onGazeHit(int source, const VROHitTestResult &hit) {
        //No-op
    }

    virtual void onSwipe(int source, SwipeState swipeState) {
        //No-op
    }

    virtual void onScroll(int source, float x, float y) {
        //No-op
    }

    virtual void onDrag(int source, VROVector3f newPosition){
        //No-op
    }

    virtual void onFuse(int source, float timeToFuseRatio){
        //No-op
    }

    void setTimeToFuse(float durationInMillis){
        _timeToFuseDuration = durationInMillis;
    }

    float getTimeToFuse(){
        return _timeToFuseDuration;
    }


private:
    std::map<VROEventDelegate::EventAction , bool> _enabledEventMap;

    /**
     * Duration used to count down from for triggering onFuse events, in milliseconds.
     * Defaults to 2000 milliseconds.
     */
    float _timeToFuseDuration = 2000;
};
#endif
