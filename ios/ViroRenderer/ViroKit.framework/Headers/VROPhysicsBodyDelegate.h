//
//  VROPhysicsBodyDelegate.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROPhysicsBodyDelegate_h
#define VROPhysicsBodyDelegate_h
#include "VROPhysicsBody.h"

/*
 VROPhysicsBodyDelegate contains all callbacks delegate events pertaining to
 the physicsBody that it is attached to (like collisions).
 */
class VROPhysicsBodyDelegate {
    /*
     LastKnownCollidedObjects is used to filter out similar collision results
     so that delegates are only notify with collided-enter events.
     */
    std::map<std::string, VROPhysicsBody::VROCollision> _lastKnownCollidedObjects;
public:
    VROPhysicsBodyDelegate(){}
    virtual ~VROPhysicsBodyDelegate() {}

    virtual void onCollided(std::string bodyBKey, VROPhysicsBody::VROCollision collision) = 0;

    /*
     Called by VROPhysicsWorld when an object has collided within the computeCollisions() pass.
     Bullet however only provides continuous contact collision checks. Instead, we only want to
     notify our delegates about collision-entered events with onCollided(). Thus, here we
     iterate through all known latestCollidedObjects and compare it with the objects we have
     last seen in _lastKnownCollidedObjects, and then only notify the delegates regarding
     new collisions.
     */
    void onEngineCollisionUpdate(std::string currentObject, const std::map<std::string, VROPhysicsBody::VROCollision> &latestCollidedObjects) {
        for (auto const &it: latestCollidedObjects) {
            if (_lastKnownCollidedObjects.find(it.first) == _lastKnownCollidedObjects.end()) {
                onCollided(it.first, it.second);
            }
        }

        _lastKnownCollidedObjects = latestCollidedObjects;
    }
};


#endif /* VROPhysicsBodyDelegate_h */
