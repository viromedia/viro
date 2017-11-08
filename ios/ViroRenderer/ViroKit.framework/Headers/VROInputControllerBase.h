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

static const float ON_DRAG_DISTANCE_THRESHOLD = 0.01;
static const float ON_PINCH_SCALE_THRESHOLD = 0.02;
static const float ON_ROTATE_THRESHOLD = 0.01; // in radians (~.5729 degrees)
static float kSceneBackgroundDistance = 8;

/*
 Responsible for mapping generalized input data from a controller, to a unified
 set of VROEventDelegate.EventTypes. It then notifies corresponding VROEventDelegates
 in the current scene about the event type that has been triggered.
 
 For example, VROInputControllerDaydream maps the onTouchPadClick event onto
 a Viro onPrimaryClick event type within VROInputControllerBase, of which then notifies all
 VROEventDelegates about such an event.
 */
class VROInputControllerBase {
public:
    VROInputControllerBase();
    virtual ~VROInputControllerBase(){}
    
    /*
     For testing background reticle distance.
     */
    void debugMoveReticle();

    /*
     onProcess is to be implemented by derived classes to drive the processing
     of platform-specific input events and map them to viro-specific input events.
     */
    virtual void onProcess(const VROCamera &camera) {
        //No-op
    }

    /*
     Called when the renderer is about to be backgrounded within Android's lifecycle.
     */
    virtual void onPause() {
        // No-op
    }

    /*
     Called when the renderer is about to be foregrounded within Android's lifecycle.
     */
    virtual void onResume() {
        // No-op
    }

    void attachScene(std::shared_ptr<VROScene> scene) {
        _scene = scene;
    }
    void detachScene() {
        _scene = nullptr;
    }

    /*
     Get the presenter, creating it if it does not yet exist. Must be invoked on the
     rendering thread.
     */
    std::shared_ptr<VROInputPresenter> getPresenter() {
        if (!_controllerPresenter) {
            _controllerPresenter = createPresenter();
            registerEventDelegate(_controllerPresenter);
        }
        return _controllerPresenter;
    }

    virtual std::string getHeadset() = 0;
    virtual std::string getController() = 0;

   /*
    For notifying components outside the scene tree, we specifically register
    them here to be tracked by the VROEventManager. Calling registerEventDelegate
    twice with the same delegate will only have callbacks be triggered once.
    */
    void registerEventDelegate(std::shared_ptr<VROEventDelegate> delegate){
        _delegates.insert(delegate);
    }

    void removeEventDelegate(std::shared_ptr<VROEventDelegate> delegate){
        _delegates.erase(delegate);
    }

    /*
     Below are Viro-specific input events to be trigged by derived Input Controller
     classes; these are the Viro-sepcific events that platform-specific events
     are mapped to.
     */
    void onControllerStatus(int source, VROEventDelegate::ControllerStatus status);
    void onButtonEvent(int source, VROEventDelegate::ClickState clickAction);
    void onTouchpadEvent(int source, VROEventDelegate::TouchState touchAction, float lastKnownX, float lastKnownY);
    
    /*
     The following position, rotation and forward are all in world coordinates.
     */
    void onMove(int source, VROVector3f position, VROQuaternion rotation, VROVector3f forward);
    void onSwipe(int source, VROEventDelegate::SwipeState swipeState);
    void onScroll(int source, float x, float y);
    
    /*
     Pinch event that passes scale factor indicting the change in the pinch ratio
     since the pinch started. Scale factor begins at 1 when pinch starts with
     PinchState::PinchStart.
     */
    void onPinch(int source, float scaleFactor, VROEventDelegate::PinchState pinchState);
    
    void onRotate(int source, float rotationRadians, VROEventDelegate::RotateState rotateState);

protected:
    
    virtual std::shared_ptr<VROInputPresenter> createPresenter(){
        perror("Error: Derived class should create a presenter for BaseInputController to consume!");
        return nullptr;
    }
    
    /*
     This function returns the forward offset used in drag
     */
    virtual VROVector3f getDragForwardOffset() = 0;
    
    /*
     Status of the current controller, for example if it's Connected / Disconnected.
     */
    VROEventDelegate::ControllerStatus _currentControllerStatus;

    /*
     Update the hit node, performing an intersection from the camera's position
     toward the given direction.
     */
    void updateHitNode(const VROCamera &camera, VROVector3f origin, VROVector3f ray);

    /*
     VRODraggedObject encapsulates all the information that needs to be tracked
     and processed for onDrag events for a given dragged node.
     */
    struct VRODraggedObject{
        std::shared_ptr<VRONode> _draggedNode;
        VROVector3f _originalHitLocation;
        VROVector3f _originalDraggedNodePosition;
        VROVector3f _forwardOffset;
        float _draggedDistanceFromController;
    };
    
    /*
     Last hit result that we are performing a drag event on.
     */
    std::shared_ptr<VRODraggedObject> _lastDraggedNode;

    /*
     This function is meant to be called to run the dragging logic after onMove
     deals with other events, etc. This allows for the dragging logic to be overridden.
     */
    virtual void processDragging(int source);
    
    /*
     Last result that was returned from the hit test.
     */
    std::shared_ptr<VROHitTestResult> _hitResult;
    
    /*
     Last known posiiton of the controller.
     */
    VROVector3f _lastKnownPosition;
    
    /*
     Last known position of the node that was dragged previously by this controller.
     */
    VROVector3f _lastDraggedNodePosition;
    
    /*
     The pointer's normalized forward vector indicating where the controller
     is pointing.
     */
    VROVector3f _lastKnownForward;

    /*
     Last known pinch scale value.
     */
    float _lastPinchScale;
    
    /*
     Last known rotation value in radians.
     */
    float _lastRotation;

    /*
     Delegates registered within the manager to be notified of events
     to an element that is outside the scene tree.
     */
    std::set<std::shared_ptr<VROEventDelegate>> _delegates;
    
    std::shared_ptr<VROScene> _scene;
    
private:
    
    /*
     UI presenter for this input controller.
     */
    std::shared_ptr<VROInputPresenter> _controllerPresenter;
    
    /*
     Last known position that a TouchEvent occured on.
     */
    VROVector3f _lastTouchedPosition;
    
    /*
     The controller's quaternion that represents the rotation from (0, 0, -1) required to
     achieve the controller's current orientation.
     */
    VROQuaternion _lastKnownRotation;

    /*
     Last node that we have clicked down on.
     */
    std::shared_ptr<VRONode> _lastClickedNode;

    /*
     Last known that was successfully hovered upon.
     */
    std::shared_ptr<VRONode> _lastHoveredNode;

    /*
     Returns the hit test result for the closest node that was hit.
     */
    VROHitTestResult hitTest(const VROCamera &camera, VROVector3f origin, VROVector3f ray, bool boundsOnly);

    /*
     Returns the first node that is able to handle the event action by bubbling it up.
     If nothing is able to handle the event, nullptr is returned.
     */
    std::shared_ptr<VRONode> getNodeToHandleEvent(VROEventDelegate::EventAction action,
                                                  std::shared_ptr<VRONode> startingNode);

    void processGazeEvent(int source, std::shared_ptr<VRONode> node);

    /*
     Current node that we are fusing on.
     */
    std::shared_ptr<VRONode> _currentFusedNode;
    
    /*
     Current node that we are pinching on.
     */
    std::shared_ptr<VRONode> _currentPinchedNode;
    
    /*
     Current node that we are rotating on.
     */
    std::shared_ptr<VRONode>  _currentRotateNode;

    /*
     Time at which the onFuse event is triggered, in milliseconds.
     */
    double _fuseTriggerAtMillis = -1;

    /*
     True if we have already notified delegates about the onFuse event.
     */
    bool _haveNotifiedOnFuseTriggered;
    void processOnFuseEvent(int source, std::shared_ptr<VRONode> node);
    void notifyOnFuseEvent(int source, float timeToFuseRatio);
};

#endif
