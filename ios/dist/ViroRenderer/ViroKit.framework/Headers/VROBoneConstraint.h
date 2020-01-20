//
//  VROBoneConstraint.h
//  ViroRenderer
//
//  Copyright © 2019 Viro Media. All rights reserved.
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
