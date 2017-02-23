//
//  VROInputControllerBase.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROInputControllerBase_h
#define VROInputControllerBase_h

#include <stdio.h>
#include <vector>
#include <string>
#include <memory>
#include <set>
#include <float.h>
#include "VROInputPresenter.h"
#include "VROScene.h"
#include "VRORenderContext.h"
#include "VROEventDelegate.h"
#include "VROHitTestResult.h"
#include "VRONode.h"
#include "VROGeometry.h"
static const float SCENE_BACKGROUND_DIST = 5.0f;
static const VROVector3f CONTROLLER_DEFAULT_POSITION = {0,0,0};
static const float ON_DRAG_DISTANCE_THRESHOLD = 0.01;
/**
 * Responsible for mapping generalized input data from a controller, to a unified
 * set of VROEventDelegate.EventTypes. It then notifies corresponding VROEventDelegates
 * in the current scene about the event type that has been triggered.
 *
 * For example, VROInputControllerDaydream maps the onTouchPadClick event onto
 * a Viro onPrimaryClick event type within VROInputControllerBase, of which then notifies all
 * VROEventDelegates about such an event.
 */
class VROInputControllerBase{
public:
    VROInputControllerBase(){
        _lastKnownPosition = VROVector3f(0,0,0);
        _lastDraggedNodePosition = VROVector3f(0,0,0);
        _lastClickedNode = nullptr;
        _lastHoveredNode = nullptr;
        _lastDraggedNode = nullptr;
        _scene = nullptr;
        _currentControllerStatus = VROEventDelegate::ControllerStatus::Unknown;
    }
    virtual ~VROInputControllerBase(){}

    /**
     * The context is attached within the construction of VRORenderer.
     */
    void setContext(std::shared_ptr<VRORenderContext> context){
        _context = context;
        _controllerPresenter = createPresenter(context);
        registerEventDelegate(_controllerPresenter);
    }

    /**
     * onProcess is to be implemented by derived classes to drive the processing
     * of platform-specific input events and map them to viro-specific input events.
     */
    virtual void onProcess(){
        //No-op
    }

    /**
     * Called when the renderer is about to be backgrounded within Android's lifecycle.
     */
    virtual void onPause(){
        // No-op
    }

    /**
     * Called when the renderer is about to be foregrounded within Android's lifecycle.
     */
    virtual void onResume(){
        // No-op
    }

    void attachScene(std::shared_ptr<VROScene> scene){
        _scene = scene;
        _scene->setControllerPresenter(_controllerPresenter);
    }

    std::shared_ptr<VROInputPresenter> getPresenter(){
        return _controllerPresenter;
    }

    virtual std::string getHeadset() = 0;
    virtual std::string getController() = 0;

    /**
    * For notifying components outside the scene tree, we specifically register
    * them here to be tracked by the VROEventManager. Calling registerEventDelegate
    * twice with the same delegate will only have callbacks be triggered once.
    */
    void registerEventDelegate(std::shared_ptr<VROEventDelegate> delegate){
        _delegates.insert(delegate);
    }

    void removeEventDelegate(std::shared_ptr<VROEventDelegate> delegate){
        _delegates.erase(delegate);
    }

    /*
     * Viro-specific input events to be trigged by derived Input Controller classes; these
     * are the Viro-sepcific events that platform-specific events are mapped to.
     */
    void onControllerStatus(int source, VROEventDelegate::ControllerStatus status);
    void onButtonEvent(int source, VROEventDelegate::ClickState clickAction);
    void onTouchpadEvent(int source, VROEventDelegate::TouchState touchAction, float lastKnownX, float lastKnownY);
    void onMove(int source, VROVector3f position, VROQuaternion rotation);
    void onSwipe(int source, VROEventDelegate::SwipeState swipeState);
    void onScroll(int source, float x, float y);
protected:
    virtual std::shared_ptr<VROInputPresenter> createPresenter(std::shared_ptr<VRORenderContext> context){
        perror("Error: Derived class should create a presenter for BaseInputController to consume!");
        return nullptr;
    }

    /**
     * UI presenter for this input controller.
     */
    std::shared_ptr<VROInputPresenter> _controllerPresenter;

    /**
     * The context is attached within the construction of VRORenderer.
     */
    std::shared_ptr<VRORenderContext> _context;

    /**
     * Status of the current controller, for example if it's Connected / Disconnected.
     */
    VROEventDelegate::ControllerStatus _currentControllerStatus;

    void updateHitNode(VROVector3f fromPosition, VROVector3f withDirection);

private:
    /*
     * The pointer's normalized forward vector indicating where the controller
     * is pointing.
     */
    VROVector3f _lastKnownForward;
    /**
     * Last known position that a TouchEvent occured on.
     */
    VROVector3f _lastTouchedPosition;
    /*
     * The controller's quaternion that represents the rotation from (0, 0, -1) required to
     * achieve the controller's current orientation.
     */
    VROQuaternion _lastKnownRotation;

    VROVector3f _lastKnownPosition;

    /*
     * Last known position of the node that was dragged previously by this controller.
     */
    VROVector3f _lastDraggedNodePosition;

    std::shared_ptr<VROScene> _scene;

    /*
     * Last result that was returned from the hit test.
     */
    std::shared_ptr<VROHitTestResult> _hitResult;

    /*
     * Last node that we have clicked down on.
     */
    std::shared_ptr<VRONode> _lastClickedNode;

    /*
     * Last known that was successfully hovered upon.
     */
    std::shared_ptr<VRONode> _lastHoveredNode;

    /**
     * Delegates registered within the manager to be notified of events
     * to an element that is outside the scene tree.
     */
    std::set<std::shared_ptr<VROEventDelegate>> _delegates;

    /**
     * Returns the closest node that was hit.
     */
    VROHitTestResult hitTest(VROVector3f vector, VROVector3f hitFromPosition, bool boundsOnly);

    /**
     * Returns the first node that is able to handle the event action by bubbling it up.
     * If nothing is able to handle the event, nullptr is returned.
     */
    std::shared_ptr<VRONode> getNodeToHandleEvent(VROEventDelegate::EventAction action,
                                                  std::shared_ptr<VRONode> startingNode);

    void processGazeEvent(int source, std::shared_ptr<VRONode> node);

    /**
     * VRODraggedObject encapsulates all the information that needs to be tracked
     * and processed for onDrag events for a given dragged node.
     */
    struct VRODraggedObject{
        std::shared_ptr<VRONode> _draggedNode;
        VROVector3f _draggedOffsetFromPointer;
        float _draggedDistanceFromController;
    };

    /*
     * Last hit result that we are performing a drag event on.
     */
    std::shared_ptr<VRODraggedObject> _lastDraggedNode;
};

#endif
