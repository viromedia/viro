//
//  VROARAnchor.h
//  ViroKit
//
//  Created by Raj Advani on 6/6/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

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
class VROARAnchor {
public:
    
    /*
     Create a new anchor.
     */
    VROARAnchor() {}
    virtual ~VROARAnchor() {}
    
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
     Update the anchor's node's transforms given the data in the anchor.
     */
    void updateNodeTransform() {
        if (_node) {
            VROVector3f scale = getTransform().extractScale();
            VROQuaternion rotation = getTransform().extractRotation(scale);
            VROVector3f position = getTransform().extractTranslation();
            
            _node->setScale(scale);
            _node->setRotation(rotation);
            _node->setPosition(position);
        }
    }
    
private:
    
    VROMatrix4f _transform;
    
    /*
     The node associated with this anchor.
     */
    std::shared_ptr<VROARNode> _node;
    
};

#endif /* VROARAnchor_h */
