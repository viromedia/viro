//
//  VROPhysicsBody.h
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

#ifndef VROPhysicsBody_h
#define VROPhysicsBody_h

#include <atomic>
#include "VROVector3f.h"
#include "VROPhysicsShape.h"
#include "VROLog.h"
#include "VROStringUtil.h"

class VRONode;
class btTransform;
class btMotionState;
class btRigidBody;
class btVector3;
class VROPhysicsBodyDelegate;

//Atomic counter used to grab a unique Id to represent a VROPhysicsBody.
static std::atomic_int sPhysicsBodyIdCounter;

/*
 VROPhysicsBody contains all the physics properties and forces that are associated with and/or
 applied to a given node. After construction, it is given to and processed within a simulated
 VROPhysicsWorld.
 */
class VROPhysicsBody : public std::enable_shared_from_this<VROPhysicsBody> {
public:
    enum class VROPhysicsBodyType {
        /*
         Static bodies are stationary, has 0 mass, and will not move.
         It can only collide with dynamic bodies.
         */
        Static = 0,

        /*
         Kinematic bodies can be moved via animations, and has 0 mass.
         It can only collide with dynamic bodies (but cannot be influenced by them).
        */
        Kinematic = 1,

        /*
         Dynamic bodies are designed to be moved only under a physics simulation,
         and has positive mass. It collides with all VROPhysicsBody types.
         */
        Dynamic = 2
    };
    static const std::string kDynamicTag;
    static const std::string kKinematicTag;
    static const std::string kStaticTag;

    /*
     Returns true if the given string and mass represents a valid representation of
     VROPhysicsBodyType. Else, false is returned and the errorMsg is populated
     with the reason for failure.
     */
    static bool isValidType(std::string strType, float mass, std::string &errorMsg) {
        if (!VROStringUtil::strcmpinsensitive(strType, kKinematicTag)
            && !VROStringUtil::strcmpinsensitive(strType, kDynamicTag)
            && !VROStringUtil::strcmpinsensitive(strType, kStaticTag)) {
            errorMsg = "Provided invalid physicsBody of type: " + strType;
            return false;
        } else if ((VROStringUtil::strcmpinsensitive(strType, kKinematicTag) || VROStringUtil::strcmpinsensitive(strType, kStaticTag)) && mass != 0) {
            errorMsg = "Mass must be 0 for kinematic or static bodies.";
            return false;
        } else if (VROStringUtil::strcmpinsensitive(strType, kDynamicTag) && mass <= 0) {
            errorMsg = "Mass must be > 0 for dynamic bodies.";
            return false;
        }

        return true;
    }

    /*
     Returns a VROPhysicsBodyType for a given string.
     */
    static VROPhysicsBody::VROPhysicsBodyType getBodyTypeForString(std::string strType) {
        if (VROStringUtil::strcmpinsensitive(strType, kKinematicTag)) {
            return VROPhysicsBody::VROPhysicsBodyType::Kinematic;
        } else if (VROStringUtil::strcmpinsensitive(strType, kDynamicTag)) {
            return VROPhysicsBody::VROPhysicsBodyType::Dynamic;
        }
        return VROPhysicsBody::VROPhysicsBodyType::Static;
    }

    VROPhysicsBody(std::shared_ptr<VRONode> node, VROPhysicsBody::VROPhysicsBodyType type,
                   float mass, std::shared_ptr<VROPhysicsShape> shape);
    virtual ~VROPhysicsBody();

    /*
     Unique key identifier that the VROPhysicsWorld uses to track this VROPhysicsBody.
     */
    std::string getKey();

    /*
     Returns a non-unique tag identifier stored in VRONode for referring to this VROPhysicsbody.
     */
    std::string getTag();
    
    /*
     Setters and getters for physics properties associated with this VROPhysicsBody.
     */
    void setMass(float mass);
    void setInertia(VROVector3f inertia);
    void setIsSimulated(bool enabled);
    bool getIsSimulated();
    void setRestitution(float restitution);
    void setUseGravity(bool useGravity);
    bool getUseGravity();
    void setFriction(float friction);
    void setType(VROPhysicsBodyType type, float mass);

    /*
     Sets this physics body in a kinematic drag mode, where we momentarily treat the body as
     a draggable kinematic object.
     */
    void setKinematicDrag(bool isDragging);

    /*
     Sets the given VROPhysicsShape that will be used to process collisions.
     */
    void setPhysicsShape(std::shared_ptr<VROPhysicsShape> shape);

    /*
     Schedules an update that re-invalidates the properties of this
     physics body on the next compute physics step.
     */
    void refreshBody();

    /*
     Functions for applying forces on this VROPhysicsBody.
     */
    void applyForce(VROVector3f power, VROVector3f position);
    void applyImpulse(VROVector3f impulse, VROVector3f position);

    /*
     Functions for applying torque on this VROPhysicsBody.
     */
    void applyTorque(VROVector3f torque);
    void applyTorqueImpulse(VROVector3f impulse);
    void clearForces();

    /*
     Sets a velocity on this VROPhysicsBody to be applied when VROPhysicsWorld calls
     applyPresetVelocity on this physics body.
     */
    void setVelocity(VROVector3f velocity, bool isConstant);

    /*
     Returns the underlying bullet rigid body that represents this VROPhysicsBody.
     */
    btRigidBody* getBulletRigidBody();

    /*
     Called to synchronize the world transform of the node associated with this VROPhysicsBody
     on to the underlying bullet physics body's transform.
     */
    void getWorldTransform(btTransform& centerOfMassWorldTrans ) const;

    /*
     Called to synchronize the world transform of the underlying bullet physics body, on to the
     node's transform associated with this VROPhysicsBody.
     */
    void setWorldTransform(const btTransform& centerOfMassWorldTrans);

    /*
     Updates the underlying Bullet physics body with the latest properties of VROPhysicsBody.
     */
    void updateBulletRigidBody();

    /*
     Flag to notify the physics world if the VROPhysicsBody has been modified (where it can then
     decide to perform updates if needed).
     */
    bool needsBulletUpdate();

    /*
     Updates the forces applied on the underlying bullet physics body. This is called and re-applied
     in each simulated physics step, as required by bullet.
     */
    void updateBulletForces();

    /*
     Applies the set velocity on this VROPhysicsBody at every physics step if isConstant was true,
     simulating constant velocity. Else, an instantaneous velocity is applied only once.
     */
    void applyPresetVelocity();

    /*
     Delegates attached to this VROPhysicsBody to be notified of collision events.
     */
    void setPhysicsDelegate(std::shared_ptr<VROPhysicsBodyDelegate> delegate);
    std::shared_ptr<VROPhysicsBodyDelegate> getPhysicsDelegate();

    /*
     Collision struct encapsulating all collision properties representing
     a collided event.
    */
    struct VROCollision {
        VROCollision(): penetrationDistance(1) { }
        VROVector3f collidedPoint;
        VROVector3f collidedNormal;
        std::string collidedBodyTag;

        /*
         Penetration depth given by bullet will be some negative number
         if they are colliding. VROCollision defaults penetrationDistance
         to a positive number (1) to ensure that it is set / re-evaulated
         in a computeCollision pass.
         */
        float penetrationDistance;
    };

private:
    std::string _key;
    std::weak_ptr<VRONode> _w_node;
    bool _needsBulletUpdate;
    btRigidBody* _rigidBody;

    // Physics Properties
    std::shared_ptr<VROPhysicsShape> _shape;
    VROPhysicsBody::VROPhysicsBodyType _type;
    bool _enableSimulation;
    float _mass;
    VROVector3f _inertia;
    bool _useGravity;
    std::weak_ptr<VROPhysicsBodyDelegate> _w_physicsDelegate;
    VROVector3f _constantVelocity;
    VROVector3f _instantVelocity;

    /*
     * Preserved physics properties when in kinematic drag mode.
     */
    float _preservedDraggedMass;
    VROPhysicsBodyType _preservedType;

    /*
     Simple force struct containing a force vector
     and the location that it is applied at.
     */
    struct BulletForce {
        VROVector3f force;
        VROVector3f location;
    };

    std::vector<BulletForce> _forces;
    std::vector<VROVector3f> _torques;

    /*
     Creates / destroys the underlying bullet object representing this VROPhysicsBody.
     */
    void createBulletBody();
    void releaseBulletBody();
};
#endif
