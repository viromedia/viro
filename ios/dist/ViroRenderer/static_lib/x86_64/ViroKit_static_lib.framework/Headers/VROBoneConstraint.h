//
//  VROBoneConstraint.h
//  ViroRenderer
//
//  Copyright © 2019 Viro Media. All rights reserved.
//

#ifndef VROBoneConstraint_h
#define VROBoneConstraint_h

#include "VROConstraint.h"
#include "VROMatrix4f.h"
class VROSkeleton;

/*
 VROBoneConstraint is applied on VRONode for constraining its position relative to a
 VROBone at all times as it is animated through the skinner.
 */
class VROBoneConstraint: public VROConstraint {
public:
    VROBoneConstraint(std::shared_ptr<VROSkeleton> skeleton,
                      int boneIndex,
                      VROMatrix4f constrainedOffsetTransform);
    VROMatrix4f getTransform(const VRORenderContext &context,
                             VROMatrix4f constrainedOffsetTransform);

    VROConstraintType getConstraintType() {
        return VROConstraintType::Bone;
    }
private:
    /*
     Bone index to which this constraint will clamp its nodes onto.
     */
    int _constraintedBoneIndex;

    /*
     An offset transform from the _constrainedBoneIndex that the constraint locks the node to
     in model space.
     */
    VROMatrix4f _constrainedOffsetTransform;

    /*
     A weak skeletal reference, used for grabbing the latest known bone transforms.
     */
    std::weak_ptr<VROSkeleton> _skeleton;
};

#endif /* VROBoneConstraint_h */
