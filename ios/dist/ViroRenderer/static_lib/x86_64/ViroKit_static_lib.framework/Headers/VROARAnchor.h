//
//  VROARAnchor.h
//  ViroKit
//
//  Created by Raj Advani on 6/6/17.
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

#ifndef VROARAnchor_h
#define VROARAnchor_h

#include "VROMatrix4f.h"
#include "VROVector3f.h"
#include "VROQuaternion.h"
#include "VROARNode.h"

/*
 Anchors are real world objects detected by the AR engine. Each time an
 anchor is detected, the VROARSessionDelegate is given an opportunity to
 create a VRONode with virtual content to attach to that anchor. The
 ARSession will thereafter ensure that the virtual content is fused with
 the real-world anchor, thereby enabling applications to 'augment'
 reality with virtual content.
 
 For example, if a plane anchor is detected, the can use its corresponding 
 VRONode to place virtual objects for a table-top game.
 
 Anchors are regularly updated by the AR engine as the characteristics of 
 the anchor are further refined: for example, the AR engine may start with
 an approximation of a surface, and later determine the surface's width and
 height.
 
 VROARAnchor is subclassed by specific anchor types; planes, image targets, 
 etc.
 */
class VROARAnchor : public std::enable_shared_from_this<VROARAnchor> {
public:
    
    /*
     Create a new anchor.
     */
    VROARAnchor() {}
    virtual ~VROARAnchor() {}
    
    /*
     String representing the ID of the anchor in the underlying platform (ARKit/ARCore).
     */
    std::string getId() const {
        return _id;
    }
    void setId(std::string id) {
        _id = id;
    }
    
    /*
     Transformation matrix encoding the position, orientation and scale of the
     anchor in world coordinates.
     */
    VROMatrix4f getTransform() const {
        return _transform;
    };
    void setTransform(VROMatrix4f transform) {
        _transform = transform;
    }
    
    /*
     The node associated with the anchor. Updated alongside the anchor.
     */
    const std::shared_ptr<VROARNode> getARNode() const {
        return _node;
    }
    void setARNode(std::shared_ptr<VROARNode> node) {
        _node = node;
        updateNodeTransform();
    }

    /*
     Get the anchor that's associated with the real-world object. On ARKit this is simply this
     anchor itself; while on ARCore the anchors have attached 'trackable' anchors.
     */
    virtual std::shared_ptr<VROARAnchor> getAnchorForTrackable() {
        return shared_from_this();
    }
    
    /*
     Update the anchor's node's transforms given the data in the anchor.
     */
    void updateNodeTransform() {
        if (_node && !_node->shouldPauseUpdates()) {
            VROVector3f scale = getTransform().extractScale();
            VROQuaternion rotation = getTransform().extractRotation(scale);
            VROVector3f position = getTransform().extractTranslation();
            
            _node->setScale(scale);
            _node->setRotation(rotation);
            _node->setPosition(position);
        }
    }
    
private:
    
    std::string _id;
    VROMatrix4f _transform;

    /*
     The node associated with this anchor.

     There is an *intentional* strong reference cycle between VROARNode and VROARAnchor. Anchors and
     ARNodes are managed in one of two ways:

     1. By the AR subsystem (ARCore). When anchors are bound to trackables, and the bound trackable
        disappears, VROARSessionARCore will remove the corresponding VROARAnchor and its VROARNode.

     2. Manually, by attaching anchors to hit results. In this case, anchors and nodes are removed
        together when the ARNode is detached from the system (see ARNode.detach() in Java).
     */
    std::shared_ptr<VROARNode> _node;
    
};

#endif /* VROARAnchor_h */
