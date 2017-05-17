//
//  VROPhysicsShape.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROPhysicsShape_h
#define VROPhysicsShape_h
#include <btBulletDynamicsCommon.h>
#include "VROGeometry.h"

/*
 VROPhysicsShape describes the type and dimensions of a physics shape that represents a VROPhysicsBody.
 */
class VROPhysicsShape {

public:
    /*
     VROShapeType describes the type of shape representing this object.
     Required parameters for each shape type are as shown below:
     */
    enum VROShapeType{
        Sphere = 1, // _params[0] represents the radius of the sphere
        Box = 2     // _params[0],[1],[2] represents the X,Y,Z half span of the Box
    };

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
