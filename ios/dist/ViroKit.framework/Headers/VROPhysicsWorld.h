//
//  VROPhysicsWorld.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

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
    void computePhysics();

    /*
     Sets the x,y,z gravity on this physics world.
     */
    void setGravity(VROVector3f gravity);
private:
    /*
     Represents the physicsBodies that have been added to and processed by this physics world.
     */
    std::set<std::shared_ptr<VROPhysicsBody>> _activePhysicsBodies;

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

};
#endif
