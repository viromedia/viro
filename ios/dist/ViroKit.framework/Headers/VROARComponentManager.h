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
#include "VROARPlane.h"

/*
 This class contains all the attach and detach logic between virtual nodes and real anchors
 that make up an AR component.
 */
class VROARComponentManager : public VROARSessionDelegate {
public:
    VROARComponentManager() {};
    virtual ~VROARComponentManager() {};
    
    // TODO: possibly update these functions to take in generic VROARNode objects
    void addARPlane(std::shared_ptr<VROARPlane> plane);
    void removeARPlane(std::shared_ptr<VROARPlane> plane);
    void updateARPlane(std::shared_ptr<VROARPlane> plane);
    
    void setSession(std::shared_ptr<VROARSession> session) {
        _session = session;
    }
    
    // VROARSessionDelegate functions
    // TODO: possibly update anchorWasDetected to *not* return a node
    virtual std::shared_ptr<VROARNode> anchorWasDetected(std::shared_ptr<VROARAnchor> anchor);
    virtual void anchorWillUpdate(std::shared_ptr<VROARAnchor> anchor);
    virtual void anchorDidUpdate(std::shared_ptr<VROARAnchor> anchor);
    virtual void anchorWasRemoved(std::shared_ptr<VROARAnchor> anchor);
    
private:
    std::weak_ptr<VROARSession> _session;
    
    // Anchors without an associated Node
    std::vector<std::shared_ptr<VROARAnchor>> _detachedAnchors;
    // Planes without an associated Anchor
    std::vector<std::shared_ptr<VROARPlane>> _detachedPlanes;
    
    /*
     Returns a detached anchor that is suitable for the given plane
     */
    std::shared_ptr<VROARAnchor> findDetachedAnchor(std::shared_ptr<VROARPlane> plane);
    
    /*
     Returns a detached plane satisfied by the given anchor
     */
    std::shared_ptr<VROARPlane> findDetachedPlane(std::shared_ptr<VROARAnchor> anchor);
    
    /*
     This functions either attaches the detached plane to an anchor or adds the plane to
     _detachedPlanes
     */
    void handleDetachedPlane(std::shared_ptr<VROARPlane> plane);
    
    /*
     This functions either attaches the detached anchor to an plane or adds the anchor to
     _detachedAnchor
     */
    void handleDetachedAnchor(std::shared_ptr<VROARAnchor> anchor);
    
    /*
     If the ARSession is still available, add the anchor's node to it
     */
    void addAnchorToSession(std::shared_ptr<VROARAnchor> anchor);
};

#endif /* VROARComponentManager_h */


