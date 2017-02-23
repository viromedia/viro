//
//  VROInputPresenter.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROInputPresenter_H
#define VROInputPresenter_H

#include <memory>
#include "VROEventDelegate.h"
#include "VRORenderContext.h"
#include "VROReticle.h"
#include "VRONode.h"
#include "VROMath.h"
#include "VROInputType.h"

static const float kReticleSizeMultiple = 3;

/**
 * VROInputPresenter contains all UI view implementations to be displayed for a given
 * VROInputController.
 */
class VROInputPresenter : public VROEventDelegate {
public:
    VROInputPresenter(std::shared_ptr<VRORenderContext> context) {
        _reticle = nullptr;
        _context = context;
        _rootNode = std::make_shared<VRONode>();
    }

    ~VROInputPresenter() {}

    std::shared_ptr<VRONode> getRootNode(){
        return _rootNode;
    }

    void setEventDelegate(std::shared_ptr<VROEventDelegate> delegate){
        _eventDelegateWeak = delegate;
    }

    /**
     * Event delegate for triggering calls back to Controller_JNI.
     */
    std::shared_ptr<VROEventDelegate> getDelegate(){
        if (_eventDelegateWeak.expired()){
            return nullptr;
        }
        return _eventDelegateWeak.lock();
    }

    virtual void onHover(int source, bool isHovering) {
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnHover)){
            delegate->onHover(source, isHovering);
        }
    }

    virtual void onClick(int source, ClickState clickState) {
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnClick)){
            delegate->onClick(source, clickState);
        }
    }

    virtual void onTouch(int source, TouchState state, float x, float y){
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnTouch)){
            delegate->onTouch(source, state, x, y);
        }
    }

    virtual void onMove(int source, VROVector3f rotation, VROVector3f position) {
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnMove)){
            delegate->onMove(source, rotation, position);
        }
    }

    virtual void onControllerStatus(int source, ControllerStatus status) {
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnControllerStatus)){
            delegate->onControllerStatus(source, status);
        }
    }

    virtual void onSwipe(int source, SwipeState swipeState) {
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnSwipe)){
            delegate->onSwipe(source, swipeState);
        }
    }

    virtual void onScroll(int source, float x, float y) {
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnScroll)){
            delegate->onScroll(source, x, y);
        }
    }

    virtual void onGazeHit(int source, float distance, VROVector3f hitLocation) {
        //No-op
    }

    virtual void onDrag(int source, VROVector3f newPosition) {
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnDrag)){
            delegate->onDrag(source, newPosition);
        }
    }

    std::shared_ptr<VROReticle> getReticle() {
            return _reticle;
    }

    void setReticle(std::shared_ptr<VROReticle> reticle){
        _reticle = reticle;
    }

protected:
    std::shared_ptr<VRONode> _rootNode;
    std::weak_ptr<VROEventDelegate> _eventDelegateWeak;

    void onReticleGazeHit(float distance, VROVector3f hitLocation){
        if (_reticle == nullptr){
            return;
        }

        float depth = -distance;

        if (_reticle->getPointerMode()){
            _reticle->setPosition(hitLocation);
        } else {
            // Lock the Reticle's position to the center of the screen
            // for non-pointer mode (usually Cardboard).
            _reticle->setPosition(VROVector3f(0, 0, depth));
        }

        float worldPerScreen = _context->getCamera().getWorldPerScreen(depth);
        float radius = fabs(worldPerScreen) * kReticleSizeMultiple;
        _reticle->setRadius(radius);
    }

private:
    std::shared_ptr<VROReticle> _reticle;
    std::shared_ptr<VRORenderContext> _context;
};
#endif
