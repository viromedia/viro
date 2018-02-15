//
//  VROPhysicsShape.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROPhysicsShape_h
#define VROPhysicsShape_h

#include "VROLog.h"
#include <memory>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>
#include "VROStringUtil.h"

class VRONode;
class btCollisionShape;
class btCompoundShape;

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
        Auto = 0,           // Automatically infer a shape from attached geometry.
        AutoCompound = 1,   // Automatically infer a compound shape from attached geometry.
        Sphere = 2,         // _params[0] represents the radius of the sphere
        Box = 3             // _params[0],[1],[2] represents the X,Y,Z half span of the Box
    };
    static const std::string kSphereTag;
    static const std::string kBoxTag;
    static const std::string kAutoCompoundTag;

    /*
     Returns true of the given string and mass represents a valid representation of
     VROPhysicsBodyType. Else, false is returned and the errorMsg is populated
     with the reason for failure.
     */
    static bool isValidShape(std::string strType, std::vector<float> params, std::string &errorMsg) {
        if (!VROStringUtil::strcmpinsensitive(strType, kSphereTag)
            && !VROStringUtil::strcmpinsensitive(strType, kBoxTag)
            && !VROStringUtil::strcmpinsensitive(strType,kAutoCompoundTag)) {
            errorMsg = "Provided invalid shape of type: " + strType;
            return false;
        } else if (VROStringUtil::strcmpinsensitive(strType, kSphereTag) && params.size() != 1) {
            errorMsg = "Invalid params provided for type sphere! Expected 1 parameter for radius.";
            return false;
        } else if (VROStringUtil::strcmpinsensitive(strType, kBoxTag) && params.size() != 3) {
            errorMsg = "Invalid params provided for type box! Expected 3 parameter defining [x,y,z].";
            return false;
        }
        return true;
    }

    static VROPhysicsShape::VROShapeType getTypeForString(std::string strType) {
        if (VROStringUtil::strcmpinsensitive(strType, kSphereTag)) {
            return VROPhysicsShape::VROShapeType::Sphere;
        } else if (VROStringUtil::strcmpinsensitive(strType, kAutoCompoundTag)) {
            return VROPhysicsShape::VROShapeType::AutoCompound;
        }
        return VROPhysicsShape::VROShapeType::Box;
    }

    VROPhysicsShape(VROShapeType type, std::vector<float> params = std::vector<float>());
    VROPhysicsShape(std::shared_ptr<VRONode> node, bool hasCompoundShapes = false);
    virtual ~VROPhysicsShape();

    /*
     Returns the Bullet representation of a VROPhysicsShape.
     */
    btCollisionShape* getBulletShape();

    /*
     Returns true if this VROShape was generated from the geometry of the
     node it is attached to.
     */
    bool getIsGeneratedFromGeometry();

    /*
     Returns true if this VROShape was generated from a combination of several
     geometric shapes (compound shape).
     */
    bool getIsCompoundShape();

private:
    /*
     Parameters that describe the dimensions of a shape.
     See VROShapeType for what parameters should be defined for which shape type.
     */
    VROShapeType _type;
    btCollisionShape* _bulletShape;

    /*
     Creates an underlying bullet collision shape representing this VROPhysicsShape,
     given the target shape type and associated params.
     */
    btCollisionShape *generateBasicBulletShape(VROShapeType type, std::vector<float> params);

    /*
     Infers from the geometry associated with the given node to create an underlying
     bullet collision shape representing this VROPhysicsShape.
     */
    btCollisionShape *generateBasicBulletShape(std::shared_ptr<VRONode> node);

    /*
     Recursively examines each node within the given root node's subtree and automatically infer
     the corresponding bullet collision shape of each node. These shapes are then combined and
     returned as a compound bullet shape.
     */
    void generateCompoundBulletShape(btCompoundShape &compoundShape,
                                     const std::shared_ptr<VRONode> &rootNode,
                                     const std::shared_ptr<VRONode> &childNode);

};
#endif
