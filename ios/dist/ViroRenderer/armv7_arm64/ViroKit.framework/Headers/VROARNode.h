//
//  VROARNode.h
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

#ifndef VROARNode_h
#define VROARNode_h

#include "VRONode.h"

class VROARAnchor;

/*
 ARNode is a specialized Node that corresponds to a detected ARAnchor. Each ARNode is
 continually updated to stay in sync with its corresponding ARAnchor: if the anchor's
 position, orientation, or other detected properties change, the ARNode will be changed
 as well.
 */
class VROARNode : public VRONode {
public:

    VROARNode() : _pauseUpdates(false) {}
    virtual ~VROARNode() {}
    
    void setAnchor(std::shared_ptr<VROARAnchor> anchor) {
        _anchor = anchor;
    }
    std::shared_ptr<VROARAnchor> getAnchor() {
        return _anchor;
    }
    
    void setPauseUpdates(bool pauseUpdates);
    bool shouldPauseUpdates() {
        return _pauseUpdates;
    }
    
protected:

    /*
     There is an *intentional* strong reference cycle between VROARNode and VROARAnchor. Anchors and
     ARNodes are managed in one of two ways:

     1. By the AR subsystem (ARCore). When anchors are bound to trackables, and the bound trackable
        disappears, VROARSessionARCore will remove the corresponding VROARAnchor and its VROARNode.

     2. Manually, by attaching anchors to hit results. In this case, anchors and nodes are removed
        together when the ARNode is detached from the system (see ARNode.detach() in Java).
     */
    std::shared_ptr<VROARAnchor> _anchor;
    bool _pauseUpdates;
    
};

#endif /* VROARNode_h */
