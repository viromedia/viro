//
//  VROARDeclarativeSession.h
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

#ifndef VROARDeclarativeSession_h
#define VROARDeclarativeSession_h

#include "VROARSession.h"
#include "VROARConstraintMatcher.h"
#include "VROARImageDatabase.h"

class VROARScene;
class VROARDeclarativeSessionDelegate;

/*
 VROARDeclarativeSession controls the 'declarative' style API for using AR.
 In this paradigm, instead of directly responding to ARSessionDelegate methods,
 the user simply specifies the requirements of the ARAnchor they're interested in
 via a VROARDeclarativeNode, and then the user can immediately start adding
 content to that node. Once an ARAnchor meeting the constraints is found, the
 node (and all of its content) will be made visible.
 
 This largely simplifies the state handling with AR objects. To use this:
 
 1. Create the VROARDeclarativeNode and set its requirements (e.g. min width and
    height for a plane).
 2. Add the node to the VROARDeclarativeSession via addARNode. This adds the node
    to the declarative session where it find an anchor that matches with it.
 3. Add the node to the VROARScene via VROARScene::addNode. This physically adds
    the node to the scene.
 
 To use the declarative session you must first call VROARScene->initDeclarativeSession().
 */
class VROARDeclarativeSession : public VROARSessionDelegate, public VROARConstraintMatcherDelegate,
                                public std::enable_shared_from_this<VROARDeclarativeSession> {
public:
    
    VROARDeclarativeSession();
    virtual ~VROARDeclarativeSession();
    
    void init();
    void setDelegate(std::shared_ptr<VROARDeclarativeSessionDelegate> delegate);

    // TODO: the following functions are also in VROARImperativeSession
    void setARSession(std::shared_ptr<VROARSession> session);
        
    void loadARImageDatabase(std::shared_ptr<VROARImageDatabase> arImageDatabase);
    void unloadARImageDatabase();
    void addARImageTarget(std::shared_ptr<VROARImageTarget> target);
    void removeARImageTarget(std::shared_ptr<VROARImageTarget> target);

    // only the declarative session has these functions b/c object tracking
    // not available for Android (ViroCore)
    void addARObjectTarget(std::shared_ptr<VROARObjectTarget> target);
    void removeARObjectTarget(std::shared_ptr<VROARObjectTarget> target);
    
    void addARNode(std::shared_ptr<VROARDeclarativeNode> plane);
    void removeARNode(std::shared_ptr<VROARDeclarativeNode> plane);
    void updateARNode(std::shared_ptr<VROARDeclarativeNode> plane);
    
    void sceneWillAppear();
    void sceneWillDisappear();

    // VROARSessionDelegate methods
    virtual void anchorWasDetected(std::shared_ptr<VROARAnchor> anchor);
    virtual void anchorWillUpdate(std::shared_ptr<VROARAnchor> anchor);
    virtual void anchorDidUpdate(std::shared_ptr<VROARAnchor> anchor);
    virtual void anchorWasRemoved(std::shared_ptr<VROARAnchor> anchor);
    
    // VROARConstraintMatcherDelegate methods
    void anchorWasAttached(std::shared_ptr<VROARAnchor> anchor);
    void anchorWasDetached(std::shared_ptr<VROARAnchor> anchor);

private:
    
    std::weak_ptr<VROARSession> _arSession;
    std::weak_ptr<VROARDeclarativeSessionDelegate> _delegate;
    std::shared_ptr<VROARConstraintMatcher> _constraintMatcher;
    std::vector<std::shared_ptr<VROARDeclarativeNode>> _nodes;
    std::vector<std::shared_ptr<VROARImageTarget>> _imageTargets;
    std::vector<std::shared_ptr<VROARObjectTarget>> _objectTargets;
    std::shared_ptr<VROARImageDatabase> _arImageDatabase;
    
};

class VROARDeclarativeSessionDelegate {
public:
    
    virtual void anchorWasDetected(std::shared_ptr<VROARAnchor> anchor) = 0;
    virtual void anchorWillUpdate(std::shared_ptr<VROARAnchor> anchor) = 0;
    virtual void anchorDidUpdate(std::shared_ptr<VROARAnchor> anchor) = 0;
    virtual void anchorWasRemoved(std::shared_ptr<VROARAnchor> anchor) = 0;
    
};

#endif /* VROARDeclarativeSession_h */
