//
//  VROPhysicsContactResultCallback.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROPhysicsMotionState_h
#define VROPhysicsMotionState_h

#include <btBulletDynamicsCommon.h>
#include "VROPhysicsBody.h"

/*
 VROPhysicsContactResultCallback is used by Bullet to return a list of collision hit results that are
 detected from a findCollisionsWithShape call.
 */
struct VROPhysicsContactResultCallback : public btCollisionWorld::ContactResultCallback {
    std::vector<VROPhysicsBody::VROCollision> _collisions;

    VROPhysicsContactResultCallback() {}
    ~VROPhysicsContactResultCallback() {_collisions.clear();}

    btScalar addSingleResult(btManifoldPoint& bulletPoint,
                             const btCollisionObjectWrapper* colObj0Wrap,
                             int partId0,
                             int index0,
                             const btCollisionObjectWrapper* colObj1Wrap,
                             int partId1,
                             int index1) {

        // Only process points that have collided
        if (bulletPoint.getDistance() > 0.f) {
            return 1.f;
        }

        // Sanity check ensuring Bullet / VROPhysics bodies are properly constructed
        const btCollisionObject* obB = colObj1Wrap->m_collisionObject;
        if (obB->getUserPointer() == nullptr) {
            perror("Incorrectly constructed bullet rigid body for a VROPhysics body!");
            return 1.f;
        }

        // Grab bullet's phsyics properties from the collided body
        const btVector3& ptB = bulletPoint.getPositionWorldOnB();
        VROVector3f collisionOnBodyB = VROVector3f(ptB.x(), ptB.y(), ptB.z());
        VROPhysicsBody *vroPhysicsBodyB = ((VROPhysicsBody *) obB->getUserPointer());
        std::string bodyKeyB = vroPhysicsBodyB->getKey();
        std::string bodyTagB = vroPhysicsBodyB->getTag();
        VROVector3f collidedNormal = VROVector3f(bulletPoint.m_normalWorldOnB.x(),
                                                 bulletPoint.m_normalWorldOnB.y(),
                                                 bulletPoint.m_normalWorldOnB.z());

        // Create and save a vector of all VROCollisions
        VROPhysicsBody::VROCollision collision;
        collision.collidedBodyTag = bodyKeyB;
        collision.collidedPoint = collisionOnBodyB;
        collision.collidedNormal = collidedNormal;
        _collisions.push_back(collision);

        // Return a 1.f hit fraction to prevent bullet from extending the hit distance.
        return 1.f;
    }
};
#endif
