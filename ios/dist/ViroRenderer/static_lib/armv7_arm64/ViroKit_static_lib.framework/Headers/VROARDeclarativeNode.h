//
//  VROARDeclarativeNode.h
//  ViroKit
//
//  Created by Raj Advani on 11/3/17.
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

#ifndef VROARDeclarativeNode_h
#define VROARDeclarativeNode_h

#include "VROARNode.h"
#include "VROARDeclarativeNodeDelegate.h"

/*
 Declarative AR nodes implement a *declarative* system for working with AR anchors.
 Instead of waiting for an anchor to appear and implementing the control flow yourself,
 you can use these nodes to simply specify the constraints of the anchor you desire,
 and the system will find the first available ARAnchor that meets those constraints
 and assign them to the node.
 
 For example, you can use VROARDeclarativePlane to specify the minimum width and height
 of the desired plane on which to play a table-top game.
 */
class VROARDeclarativeNode : public VROARNode {
public:
    
    VROARDeclarativeNode() : _isAttached(false), _internalHidden(false) {
    }
    virtual ~VROARDeclarativeNode() {}
    
    /*
     Returns true if the given anchor fulfills the requirements for this declarative
     AR node.
     */
    virtual bool hasRequirementsFulfilled(std::shared_ptr<VROARAnchor> candidate) = 0;
    
    /*
     Set an anchor ID to override the constraint system. The VROARConstraintMatcher will
     first match nodes to anchors that have matching IDs, prior to checking constraints.
     */
    void setAnchorId(std::string anchorId) {
        _anchorId = anchorId;
    }
    std::string getAnchorId() {
        return _anchorId;
    }
    
    /*
     Set a delegate to respond to the node being attached/detached/updated.
     */
    void setARNodeDelegate(std::shared_ptr<VROARDeclarativeNodeDelegate> delegate) {
        _arNodeDelegate = delegate;
    }
    std::shared_ptr<VROARDeclarativeNodeDelegate> getARNodeDelegate() {
        return _arNodeDelegate.lock();
    }
    
    /*
     Declarative AR nodes are kept hidden until they find a matching anchor.
     */
    virtual void setHidden(bool hidden) {
        _internalHidden = hidden;
        VRONode::setHidden(!_isAttached || _internalHidden);
    }
    
    /*
     Set whether this declarative AR node is attached to an anchor.
     */
    void setAttached(bool attached) {
        _isAttached = attached;
        // set hidden again when the _isAttached changes.
        setHidden(_internalHidden);
    }
    
    /*
     Invoked internally by the VROARConstraintMatcher.
     */
    virtual void onARAnchorAttached() {
        setAttached(true);
        std::shared_ptr<VROARDeclarativeNodeDelegate> delegate = getARNodeDelegate();
        if (delegate) {
            delegate->onARAnchorAttached(getAnchor());
        }
    }
    
    virtual void onARAnchorUpdated() {
        std::shared_ptr<VROARDeclarativeNodeDelegate> delegate = getARNodeDelegate();
        if (delegate) {
            delegate->onARAnchorUpdated(getAnchor());
        }
    }
    
    virtual void onARAnchorRemoved() {
        setAttached(false);
        std::shared_ptr<VROARDeclarativeNodeDelegate> delegate = getARNodeDelegate();
        if (delegate) {
            delegate->onARAnchorRemoved();
        }
    }
    
private:
    
    /*
     It is possible to override the declarative system and directly set an anchor ID.
     The VROARConstraintMatcher will first match nodes to anchors that have matching
     IDs, prior to checking constraints.
     */
    std::string _anchorId;
    
    /*
     True if the VROARConstraintMatcher has found a match for this node.
     */
    bool _isAttached;
    
    /*
     Declarative AR nodes are kept hidden until they find a matching anchor. This way
     their contents are not displayed to the user until they've been correctly anchored
     to the real world. This is achieved by overriding VRONode::setHidden().
     */
    bool _internalHidden;
    
    /*
     Delegate to respond to this node being attached / detached.
     */
    std::weak_ptr<VROARDeclarativeNodeDelegate> _arNodeDelegate;
    
};

#endif /* VROARDeclarativeNode_h */
