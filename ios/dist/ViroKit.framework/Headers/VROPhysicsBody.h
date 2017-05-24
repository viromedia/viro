//
//  VROPhysicsBody.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROPhysicsBody_h
#define VROPhysicsBody_h

#include "VROVector3f.h"
#include "VROPhysicsShape.h"
#include "VROLog.h"

class VRONode;
class btTransform;
class btMotionState;
class btRigidBody;
class btVector3;

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
        if (strType != kKinematicTag && strType != kDynamicTag && strType != kStaticTag) {
            errorMsg = "Provided invalid physicsBody of type: " + strType;
            return false;
        } else if ((strType == kKinematicTag || strType == kStaticTag) && mass !=0) {
            errorMsg = "Mass must be 0 for kinematic or static bodies.";
            return false;
        } else if (strType == kDynamicTag && mass <=0) {
            errorMsg = "Mass must be > 0 for dynamic bodies.";
            return false;
        }

        return true;
    }

    /*
     Returns a VROPhysicsBodyType for a given string.
     */
    static VROPhysicsBody::VROPhysicsBodyType getBodyTypeForString(std::string strType) {
        if (!strType.compare(kKinematicTag)) {
            return VROPhysicsBody::VROPhysicsBodyType::Kinematic;
        } else if (!strType.compare(kDynamicTag)) {
            return VROPhysicsBody::VROPhysicsBodyType::Dynamic;
        }
        return VROPhysicsBody::VROPhysicsBodyType::Static;
    }

    VROPhysicsBody(std::shared_ptr<VRONode> node, VROPhysicsBody::VROPhysicsBodyType type,
                   float mass, std::shared_ptr<VROPhysicsShape> shape);
    virtual ~VROPhysicsBody();

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

    /*
     Sets the given VROPhysicsShape that will be used to process collisions.
     */
    void setPhysicsShape(std::shared_ptr<VROPhysicsShape> shape);

    /*
     Functions for applying forces on this VROPhysicsBody.
     */
    void applyCenteralForce(VROVector3f force);
    void applyCenteralImpulse(VROVector3f impulse);

    /*
     Functions for applying torque on this VROPhysicsBody.
     */
    void applyTorque(VROVector3f torque);
    void applyTorqueImpulse(VROVector3f impulse);

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

private:
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
};
#endif
