//
//  VROARDraggableNode.h
//  ViroKit
//
//  Created by Andy Chu on 8/1/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROARDraggableNode_h
#define VROARDraggableNode_h

#include "VRONode.h"
#include "VROTransaction.h"

/*
 This is a VRONode that can be dragged along real-world surfaces/points. The logic
 for that currently lives within VROInputControllerARiOS.
 */
class VROARDraggableNode : public VRONode {
public:
    VROARDraggableNode();
    virtual ~VROARDraggableNode();

    bool isAnimating() {
        return _isAnimating;
    }

    void setAnimating(bool isAnimating) {
        _isAnimating = isAnimating;
    }

    std::shared_ptr<VROTransaction> getAnimationTransaction() {
        return _animationTransaction;
    }

    void setAnimationTransaction(std::shared_ptr<VROTransaction> animationTransaction) {
        _animationTransaction = animationTransaction;
    }

private:
    bool _isAnimating;
    std::shared_ptr<VROTransaction> _animationTransaction;
};

#endif /* VROARDraggableNode_h */
