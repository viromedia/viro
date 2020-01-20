//
//  VROPhysicsBodyDelegate.h
//  ViroRenderer
//
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

#ifndef VROPhysicsBodyDelegate_h
#define VROPhysicsBodyDelegate_h
#include "VROPhysicsBody.h"
#include "VROTime.h"

/*
 VROPhysicsBodyDelegate contains all callbacks delegate events pertaining to
 the VROPhysicsBody that it is attached to (like collisions).
 */
class VROPhysicsBodyDelegate {
    /*
     LastKnownCollidedObjects is used to filter out similar collision results
     so that delegates are only notify with collided-enter events.
     */
    std::map<std::string, VROPhysicsBody::VROCollision> _lastKnownCollidedObjects;
    std::map<std::string, VROPhysicsBody::VROCollision> _currentCollidedObjects;
    double _lastSampledTime = 0;

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
        _currentCollidedObjects.insert(latestCollidedObjects.begin(), latestCollidedObjects.end());

        // Sample at a rate of every 10 frames
        double collidedTime = VROTimeCurrentMillis();
        if (_lastSampledTime + 160 > collidedTime) {
            return;
        }

        _lastSampledTime = collidedTime;
        for (auto &it: _currentCollidedObjects) {
            if (_lastKnownCollidedObjects.find(it.first) == _lastKnownCollidedObjects.end()) {
                onCollided(it.first, it.second);
            }
        }
        
        _lastKnownCollidedObjects = _currentCollidedObjects;
        _currentCollidedObjects.clear();
    }
};


#endif /* VROPhysicsBodyDelegate_h */
