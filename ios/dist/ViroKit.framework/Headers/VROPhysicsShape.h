//
//  VROPhysicsShape.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROPhysicsShape_h
#define VROPhysicsShape_h
#include "VROGeometry.h"
#include "VROLog.h"

class btCollisionShape;

/*
 VROPhysicsShape describes the type and dimensions of a physics shape that represents a VROPhysicsBody.
 */
class VROPhysicsShape {

public:
    /*
     VROShapeType describes the type of shape representing this object.
     Required parameters for each shape type are as shown below:
     */
    enum VROShapeType {
        Sphere = 1, // _params[0] represents the radius of the sphere
        Box = 2     // _params[0],[1],[2] represents the X,Y,Z half span of the Box
    };
    static const std::string kSphereTag;
    static const std::string kBoxTag;

    /*
     Returns true of the given string and mass represents a valid representation of
     VROPhysicsBodyType. Else, false is returned and the errorMsg is populated
     with the reason for failure.
     */
    static bool isValidShape(std::string strType, std::vector<float> params, std::string &errorMsg) {
        if (strType != kSphereTag && strType != kBoxTag) {
            errorMsg = "Provided invalid shape of type: " + strType;
            return false;
        } else if (strType == kSphereTag && params.size() != 1) {
            errorMsg = "Invalid params provided for type sphere! Expected 1 parameter for radius.";
            return false;
        } else if (strType == kBoxTag && params.size() != 3) {
            errorMsg = "Invalid params provided for type box! Expected 3 parameter defining [x,y,z].";
            return false;
        }
        return true;
    }

    static VROPhysicsShape::VROShapeType getTypeForString(std::string strType) {
        if (strType == kSphereTag) {
            return VROPhysicsShape::VROShapeType::Sphere;
        }
        return VROPhysicsShape::VROShapeType::Box;
    }

    VROPhysicsShape(VROShapeType type, std::vector<float> params);
    VROPhysicsShape(std::shared_ptr<VROGeometry> geometry);
    virtual ~VROPhysicsShape();

    /*
     Returns the Bullet representation of a VROPhysicsShape.
     */
    btCollisionShape* getBulletShape();

private:
    /*
     Parameters that describe the dimensions of a shape.
     See VROShapeType for what parameters should be defined for which shape type.
     */
    std::vector<float> _params;
    VROShapeType _type;
    btCollisionShape* _bulletShape;
};
#endif
