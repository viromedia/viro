//
//  VROPhysicsWorld.h
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

#ifndef VROPhysicsWorld_h
#define VROPhysicsWorld_h
#include <memory>
#include "VROPhysicsBody.h"

class btBulletDynamicsCommon;
class btDiscreteDynamicsWorld;
class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class VROPhysicsDebugDraw;
class VRODriver;
class VRORenderContext;

/*
 VROPhysicsWorld is a simulated physics environment that contains and processes
 all acting forces and collisions on VROPhysicsBodies. It also contains both
 the physics properties of the simulated world (like gravity) and collision
 configuration parameters.
 */
class VROPhysicsWorld{
public:
    VROPhysicsWorld();
    virtual ~VROPhysicsWorld();

    /*
     Adds and removes a physics rigid body from the physics world.
     Also guards against adding or removing the same physics
     body from the world twice.
     */
    void addPhysicsBody(std::shared_ptr<VROPhysicsBody> body);
    void removePhysicsBody(std::shared_ptr<VROPhysicsBody> body);

    /*
     When called, performs a timeStep of simulation / calculations for this physics world.
     */
    void computePhysics(const VRORenderContext &context);

    /*
     Iterate through the dynamic world, identify collided object pairs and notify their corresponding
     physicsBodyDelegates regarding the collision event.
     */
    void computeCollisions();

    /*
     Sets the x,y,z gravity on this physics world.
     */
    void setGravity(VROVector3f gravity);

    /*
     Projects a ray into the scene from the given start to end location and returns
     true if it has collided with any VROPhysics shape. If a collision occurred,
     the collided body's physics delegate will be notified as well.
     */
    bool findCollisionsWithRay(VROVector3f from, VROVector3f to, bool returnClosest,
                               std::string rayTag);

    /*
     Projects a shape into the scene from the given start to end location and returns
     true if it has collided with any VROPhysics shape. If a collision occurred,
     the collided body's physics delegate will be notified as well.

     Note: If checking along a path, only the first collided object is notified. Else,
     if checking at a point (where start and end VROVector3fs are the same), all collided
     objects intersecting the shape are notified. This is currently a Bullet limitation.
     */
    bool findCollisionsWithShape(VROVector3f fromPos, VROVector3f toPos,
                                 std::shared_ptr<VROPhysicsShape> shape,
                                 std::string rayTag);

    /*
     If true, renders a set of lines representing the collision mesh of all physicsBodies
     within this world.
     */
    void setDebugDrawVisible(bool isVisible);
    
private:
    
    /*
     Represents the physicsBodies that have been added to and processed by this physics world.
     */
    std::map<std::string, std::shared_ptr<VROPhysicsBody>> _activePhysicsBodies;

    /*
     Bullet's representation of the physics world.
     */
    btDiscreteDynamicsWorld* _dynamicsWorld;

    /*
     Bullet Broadphase represents the collision algorithm used for quick, rough computations
     of collision pairs early on in the physics pipeline.
     */
    btBroadphaseInterface* _broadphase;

    /*
     Configuration used for fine tuning collision algorithms.
     */
    btDefaultCollisionConfiguration* _collisionConfiguration;

    /*
     Dispatcher is used for the notification of collisions.
     */
    btCollisionDispatcher* _collisionDispatcher;

    /*
     Represents the constraints upon which the objects in this world will be resolved against.
     This takes into account things like gravity, collisions, and hinges.
     */
    btSequentialImpulseConstraintSolver* _constraintSolver;

    /*
     Performs a collision shape test at the given location, returns true if it has collided
     with any VROPhysics shape, and notifies delegates along the way.
     */
    bool collisionTestAtPoint(VROVector3f pos, std::shared_ptr<VROPhysicsShape> shape,
                              std::string rayTag);

    /*
     Projects a shape into the scene from the given start to end location, returns
     true if it has collided with any VROPhysics shape. Only the closest VROPhysicsShape's
     delegate is notified.
     */
    bool collisionTestAlongPath(VROVector3f fromPos, VROVector3f toPos,
                                std::shared_ptr<VROPhysicsShape> shape,
                                std::string rayTag);

    /*
     Used by Bullet to render all debug elements within the physics world.
     */
    VROPhysicsDebugDraw* _debugDraw;
    bool _debugDrawVisible;
};
#endif
