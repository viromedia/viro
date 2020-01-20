//
//  VROPortalTraversalListener.h
//  ViroKit
//
//  Created by Raj Advani on 8/3/17.
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

#ifndef VROPortalTraversalListener_h
#define VROPortalTraversalListener_h

#include "VROFrameListener.h"
#include "VROTree.h"
#include <memory>

class VROScene;
class VROPortal;
class VROVector3f;
class VROLineSegment;

class VROPortalTraversalListener : public VROFrameListener {
public:
    
    VROPortalTraversalListener(std::shared_ptr<VROScene> scene);
    virtual ~VROPortalTraversalListener();
    
    void onFrameWillRender(const VRORenderContext &context);
    void onFrameDidRender(const VRORenderContext &context);
    
private:
    
    std::weak_ptr<VROScene> _scene;
    
    /*
     Scan the portal tree's immediate children to see if we've intersected any passable
     portal with the given segment. Return the portal found, if any.
     
     Note this does *not* recurse because only the portals that are immediate children
     of the current portal are passable.
     */
    std::shared_ptr<VROPortal> findPortalTraversal(const VROLineSegment &segment,
                                                   const tree<std::shared_ptr<VROPortal>> &portalTree);
    
    /*
     Scan the portal tree for passable portals, and if we're a significant distance
     away from them, restore them to one-sided mode. We only want portals we are about
     to traverse to be in two-sided mode.
     */
    void restorePortalFaces(const VROVector3f &cameraPosition,
                            const tree<std::shared_ptr<VROPortal>> &portalTree);
    
};

#endif /* VROPortalTraversalListener_h */
