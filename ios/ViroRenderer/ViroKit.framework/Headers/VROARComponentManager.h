//
//  VROARComponentManager.h
//  ViroKit
//
//  Created by Andy Chu on 6/16/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROARComponentManager_h
#define VROARComponentManager_h

#include "VROARSession.h"
#include "VROARPlaneNode.h"

class VROARComponentManagerDelegate;

/*
 This class contains all the attach and detach logic between virtual nodes and real anchors
 that make up an AR component.
 */
class VROARComponentManager : public VROARSessionDelegate {
public:
    VROARComponentManager() {};
    virtual ~VROARComponentManager() {};

    // TODO: possibly update these functions to take in generic VROARNode objects
    void addARPlane(std::shared_ptr<VROARPlaneNode> plane);
    void removeARPlane(std::shared_ptr<VROARPlaneNode> plane);
    void updateARPlane(std::shared_ptr<VROARPlaneNode> plane);

    void setDelegate(std::shared_ptr<VROARComponentManagerDelegate> delegate);

    /*
     This function will silently detach all the anchors from the given planes. This
     is currently used when ARScene is being detached.
     */
    void clearAllPlanes(std::vector<std::shared_ptr<VROARPlaneNode>> planes);
    
    // VROARSessionDelegate functions
    virtual void anchorWasDetected(std::shared_ptr<VROARAnchor> anchor);
    virtual void anchorWillUpdate(std::shared_ptr<VROARAnchor> anchor);
    virtual void anchorDidUpdate(std::shared_ptr<VROARAnchor> anchor);
    virtual void anchorWasRemoved(std::shared_ptr<VROARAnchor> anchor);
    
private:
    
    // Map between a string ID and the VROARAnchor with that ID.
    std::map<std::string, std::shared_ptr<VROARAnchor>> _nativeAnchorMap;
    std::shared_ptr<VROARAnchor> getAnchorFromId(std::string id);

    std::weak_ptr<VROARComponentManagerDelegate> _delegate;

    // Anchors without an associated Node
    std::vector<std::shared_ptr<VROARAnchor>> _detachedAnchors;
    // Planes without an associated Anchor
    std::vector<std::shared_ptr<VROARPlaneNode>> _detachedPlanes;
    
    /*
     Returns a detached anchor that is suitable for the given plane
     */
    std::shared_ptr<VROARAnchor> findDetachedAnchor(std::shared_ptr<VROARPlaneNode> plane);
    
    /*
     Returns a detached plane satisfied by the given anchor
     */
    std::shared_ptr<VROARPlaneNode> findDetachedPlane(std::shared_ptr<VROARAnchor> anchor);
    
    /*
     This function either attaches the detached plane to an anchor or adds the plane to
     _detachedPlanes
     */
    void processDetachedPlane(std::shared_ptr<VROARPlaneNode> plane);
    
    /*
     This function either attaches the detached anchor to an plane or adds the anchor to
     _detachedAnchor
     */
    void processDetachedAnchor(std::shared_ptr<VROARAnchor> anchor);

    /*
     This function handles the logic and updating required for attaching a node to an anchor
     */
    void attachNodeToAnchor(std::shared_ptr<VROARNode> node, std::shared_ptr<VROARAnchor> anchor);

    /*
     This function handles detaching a node from an anchor
     */
    void detachNodeFromAnchor(std::shared_ptr<VROARAnchor> anchor);

    /*
     Functions to abstract out the notification of the delegate
     */
    void notifyAnchorWasDetected(std::shared_ptr<VROARAnchor> anchor);
    void notifyAnchorWasUpdated(std::shared_ptr<VROARAnchor> anchor);
    void notifyAnchorWasRemoved(std::shared_ptr<VROARAnchor> anchor);

    /*
     Helper functions to remove the given objects from the _detached* vectors
     */
    void removeFromDetachedList(std::shared_ptr<VROARPlaneNode> plane);
    void removeFromDetachedList(std::shared_ptr<VROARAnchor> anchor);
};

class VROARComponentManagerDelegate {
public:
    
    /*
     Invoked whenever an anchor is detected by the underlying AR session
     */
    virtual void anchorWasDetected(std::shared_ptr<VROARAnchor> anchor) = 0;

    /*
     Invoked when an anchor is updated either by the underlying AR session or
     if it was attached to a node.
     */
    virtual void anchorWasUpdated(std::shared_ptr<VROARAnchor> anchor) = 0;
    
    /*
     Invoked when an anchor is removed from the underlying AR Session
     */
    virtual void anchorWasRemoved(std::shared_ptr<VROARAnchor> anchor) = 0;
    
};

#endif /* VROARComponentManager_h */


