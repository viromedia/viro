//
//  VROPhysicsMotionState.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROPhysicsMotionState_h
#define VROPhysicsMotionState_h

#include <btBulletDynamicsCommon.h>

/*
 VROPhysicsMotionState, when attached to a Bullet body, notifies or grabs
 transformation updates to/from a weakly referenced VROPhysicsBody.
 */
class VROPhysicsMotionState : public btMotionState {
public:
    std::weak_ptr<VROPhysicsBody> _w_physicsBody;

    VROPhysicsMotionState(std::shared_ptr<VROPhysicsBody> body) {
        _w_physicsBody = body;
    }
    virtual ~VROPhysicsMotionState(){}

    void getWorldTransform(btTransform& centerOfMassWorldTrans ) const {
        std::shared_ptr<VROPhysicsBody> body = _w_physicsBody.lock();
        if (!body) {
            return;
        }

        body->getWorldTransform(centerOfMassWorldTrans);
    }

    void setWorldTransform(const btTransform& centerOfMassWorldTrans) {
        std::shared_ptr<VROPhysicsBody> body = _w_physicsBody.lock();
        if (!body) {
            return;
        }

        body->setWorldTransform(centerOfMassWorldTrans);
    }
};
#endif
