//
//  VROInputPresenter.h
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

#ifndef VROInputPresenter_H
#define VROInputPresenter_H

#include <memory>
#include "VROAtomic.h"
#include "VROEventDelegate.h"
#include "VRORenderContext.h"
#include "VROReticle.h"
#include "VRONode.h"
#include "VROMath.h"
#include "VROInputType.h"
#include "VROThreadRestricted.h"

static const float kReticleSizeMultiple = 3;
static const bool kDebugSceneBackgroundDistance = false;

/*
 VROInputPresenter contains all UI view implementations to be displayed for a given
 VROInputController.
 */
class VROInputPresenter : public VROEventDelegate, public VROThreadRestricted {
public:
    
    VROInputPresenter() : VROThreadRestricted(VROThreadName::Renderer) {
        _reticle = nullptr;
        _rootNode = std::make_shared<VRONode>();
    }

    ~VROInputPresenter() {}

    std::shared_ptr<VRONode> getRootNode(){
        return _rootNode;
    }

    void setEventDelegate(std::shared_ptr<VROEventDelegate> delegate){
        _eventDelegateWeak = delegate;
    }

    virtual void onHover(int source, std::shared_ptr<VRONode> node, bool isHovering, std::vector<float> position) {
        passert_thread(__func__);
        
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnHover)){
            delegate->onHover(source, node, isHovering, position);
        }
    }

    virtual void onClick(int source, std::shared_ptr<VRONode> node, ClickState clickState, std::vector<float> position) {
        passert_thread(__func__);
        
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnClick)){
            delegate->onClick(source, node, clickState, position);
        }
    }

    virtual void onTouch(int source, std::shared_ptr<VRONode> node, TouchState state, float x, float y){
        passert_thread(__func__);
        
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnTouch)){
            delegate->onTouch(source, node, state, x, y);
        }
    }

    virtual void onMove(int source, std::shared_ptr<VRONode> node,
                        VROVector3f rotation, VROVector3f position, VROVector3f forwardVec) {
        passert_thread(__func__);
        _lastKnownForward = forwardVec;

        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnMove)){
            delegate->onMove(source, node, rotation, position, forwardVec);
        }
    }

    virtual void onControllerStatus(int source, ControllerStatus status) {
        passert_thread(__func__);

        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnControllerStatus)){
            delegate->onControllerStatus(source, status);
        }
    }

    virtual void onSwipe(int source, std::shared_ptr<VRONode> node, SwipeState swipeState) {
        passert_thread(__func__);

        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnSwipe)){
            delegate->onSwipe(source, node, swipeState);
        }
    }

    virtual void onScroll(int source, std::shared_ptr<VRONode> node, float x, float y) {
        passert_thread(__func__);
        
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnScroll)){
            delegate->onScroll(source, node, x, y);
        }
    }

    virtual void onGazeHit(int source, std::shared_ptr<VRONode> node, const VROHitTestResult &hit) {
        //No-op
    }

    virtual void onDrag(int source, std::shared_ptr<VRONode> node, VROVector3f newPosition) {
        passert_thread(__func__);
        
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnDrag)){
            delegate->onDrag(source, node, newPosition);
        }
    }

    virtual void onFuse(int source, std::shared_ptr<VRONode> node, float timeToFuseRatio) {
        passert_thread(__func__);
        if (_reticle == nullptr) {
            return;
        }

        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnFuse)) {
            delegate->onFuse(source, node, timeToFuseRatio);
        }

        // TimeToFuseRatio is (time that has passed since fuse began) / (total time to fuse).
        // When the timeToFuseRatio reaches 1, it is an indication that the node has been "onFused".
        if (timeToFuseRatio == kOnFuseReset) {
            _reticle->stopFuseAnimation();
        } else {
            _reticle->animateFuse(1 - timeToFuseRatio);
        }
    }

    std::shared_ptr<VROReticle> getReticle() {
        return _reticle;
    }

    void setReticle(std::shared_ptr<VROReticle> reticle){
        _reticle = reticle;
        _reticleInitialPositionSet = false;
    }

    VROVector3f getLastKnownForward() {
        return _lastKnownForward;
    }

    void updateLastKnownForward(VROVector3f forward) {
        _lastKnownForward = forward;
    }

protected:
    
    std::shared_ptr<VRONode> _rootNode;

    void onReticleGazeHit(const VROHitTestResult &hit) {
        passert_thread(__func__);
        if (_reticle == nullptr) {
            return;
        }

        float depth = -hit.getDistance();

        if (!_reticle->isHeadlocked()) {
            _reticle->setPosition(hit.getLocation());
            
            float worldPerScreen = hit.getCamera().getWorldPerScreen(depth);
            float radius = fabs(worldPerScreen) * kReticleSizeMultiple;
            _reticle->setRadius(radius);
        }
        else {
            // Lock the Reticle's position to the center of the screen
            // for fixed pointer mode (usually Cardboard). The reticle is
            // rendered as HUD object, with view matrix identity (e.g. it
            // always follows the headset)
            
            // Set the 'depth' of the reticle to the object it is hovering
            // over, then set the radius to compensate for that distance so
            // that the reticle stays the same size. The depth effectively
            // determines the difference in reticle position between the two
            // eyes.
            
            // Only use the background depth if this is our first time
            // positioning the reticle. Otherwise we maintain the current
            // reticle depth, to avoid reticle 'popping' that occurs when
            // the user moves from an actual focused object to the background.
            // The background has no 'actual' depth so this is ok.
            if (!_reticleInitialPositionSet || !hit.isBackgroundHit() || kDebugSceneBackgroundDistance) {
                _reticle->setPosition(VROVector3f(0, 0, depth));
                _reticleInitialPositionSet = true;
                
                float worldPerScreen = hit.getCamera().getWorldPerScreen(depth);
                float radius = fabs(worldPerScreen) * kReticleSizeMultiple;
                _reticle->setRadius(radius);
            }
        }
    }

private:

    std::weak_ptr<VROEventDelegate> _eventDelegateWeak;

    std::shared_ptr<VROReticle> _reticle;
    bool _reticleInitialPositionSet;
    VROAtomic<VROVector3f> _lastKnownForward;

    /*
     Event delegate for triggering calls back to Controller_JNI.
     */
    std::shared_ptr<VROEventDelegate> getDelegate(){
        if (_eventDelegateWeak.expired()){
            return nullptr;
        }
        return _eventDelegateWeak.lock();
    }
    
};
#endif
