//
//  VROARConstraintMatcher.h
//  ViroKit
//
//  Created by Andy Chu on 6/16/17.
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

#ifndef VROARConstraintMatcher_h
#define VROARConstraintMatcher_h

#include "VROARSession.h"
#include <map>
#include <vector>

class VROARAnchor;
class VROARDeclarativeNode;
class VROARConstraintMatcherDelegate;

/*
 VROARConstraintMatcher handles matching declarative nodes with anchors that
 meet their requirements.
 */
class VROARConstraintMatcher {
public:
    VROARConstraintMatcher() {};
    virtual ~VROARConstraintMatcher() {};

    void addARNode(std::shared_ptr<VROARDeclarativeNode> node);
    void removeARNode(std::shared_ptr<VROARDeclarativeNode> node);
    void updateARNode(std::shared_ptr<VROARDeclarativeNode> node);

    void setDelegate(std::shared_ptr<VROARConstraintMatcherDelegate> delegate);

    /*
     This function will silently detach all the anchors from the given nodes. This
     is currently used when ARScene is being detached.
     */
    void detachAllNodes(std::vector<std::shared_ptr<VROARDeclarativeNode>> nodes);
    
    void anchorWasDetected(std::shared_ptr<VROARAnchor> anchor);
    void anchorDidUpdate(std::shared_ptr<VROARAnchor> anchor);
    void anchorWasRemoved(std::shared_ptr<VROARAnchor> anchor);
    
private:
    
    // Map between a string ID and the VROARAnchor with that ID.
    std::map<std::string, std::shared_ptr<VROARAnchor>> _nativeAnchorMap;
    std::shared_ptr<VROARAnchor> getAnchorFromId(std::string id);

    std::weak_ptr<VROARConstraintMatcherDelegate> _delegate;

    // Anchors without an associated Node
    std::vector<std::shared_ptr<VROARAnchor>> _detachedAnchors;

    // Nodes w/o id's without an associated Anchor
    std::vector<std::shared_ptr<VROARDeclarativeNode>> _detachedNodes;

    // Nodes w/ id's without an associated Anchor
    std::vector<std::shared_ptr<VROARDeclarativeNode>> _detachedNodesWithID;

    /*
     Returns a detached anchor that is suitable for the given node.
     */
    std::shared_ptr<VROARAnchor> findDetachedAnchor(std::shared_ptr<VROARDeclarativeNode> node);
    
    /*
     Returns a detached node satisfied by the given anchor.
     */
    std::shared_ptr<VROARDeclarativeNode> findDetachedNode(std::shared_ptr<VROARAnchor> anchor);
    
    /*
     This function either attaches the detached node to an anchor or adds the node to
     _detachedNodes.
     */
    void processDetachedNode(std::shared_ptr<VROARDeclarativeNode> node);
    
    /*
     This function either attaches the detached anchor to an node or adds the anchor to
     _detachedAnchors.
     */
    void processDetachedAnchor(std::shared_ptr<VROARAnchor> anchor);

    /*
     This function handles the logic and updating required for attaching a node to an anchor
     */
    void attachNodeToAnchor(std::shared_ptr<VROARDeclarativeNode> node, std::shared_ptr<VROARAnchor> anchor);

    /*
     This function handles detaching a node from an anchor
     */
    void detachNodeFromAnchor(std::shared_ptr<VROARAnchor> anchor);

    /*
     Functions to abstract out the notification of the delegate
     */
    void notifyAnchorWasAttached(std::shared_ptr<VROARAnchor> anchor);
    void notifyAnchorWasDetached(std::shared_ptr<VROARAnchor> anchor);

    /*
     Helper functions to remove the given objects from the _detached* vectors
     */
    void removeFromDetachedList(std::shared_ptr<VROARDeclarativeNode> node);
    void removeFromDetachedList(std::shared_ptr<VROARAnchor> anchor);
};

class VROARConstraintMatcherDelegate {
public:

    /*
     Invoked when an anchor is attached to node.
     */
    virtual void anchorWasAttached(std::shared_ptr<VROARAnchor> anchor) = 0;
    
    /*
     Invoked when an anchor is detached from a node.
     */
    virtual void anchorWasDetached(std::shared_ptr<VROARAnchor> anchor) = 0;
    
};

#endif /* VROARConstraintMatcher_h */


