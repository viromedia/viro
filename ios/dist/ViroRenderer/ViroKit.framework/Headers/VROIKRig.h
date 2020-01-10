//
//  VROIKRig.h
//  ViroRenderer
//
//  Copyright © 2018 Viro Media. All rights reserved.
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

#ifndef VROIKRig_h
#define VROIKRig_h

#include <memory>
#include <vector>
#include <map>
#include "VROMatrix4f.h"
#include "VROVector3f.h"
#include "VROQuaternion.h"

class VRONode;
class VROSkeleton;

/*
 A joint in the Inverse Kinematic Rig used for performing and storing inverse kinematic
 calculations.
 */
struct VROIKJoint {
    // Unique id representing this VROIKJoint
    int id;

    // Transforms representing this joint in world space.
    VROVector3f position;
    VROQuaternion rotation;
    VROVector3f scale;

    // Actual VRONode representing this VROBoneNode.
    std::shared_ptr<VRONode> syncNode;
    int syncBone;

    // True if this joint is a junction upon which multiple IKChains converge.
    bool isCentroidJoint;
    std::vector<VROVector3f> centroidSubLocations;

    // True if this joint is an intermediary effector.
    bool isIntermediaryEffector;

    // Pointers to parent and child IKJointNodes
    std::shared_ptr<VROIKJoint> parent;
    std::vector<std::shared_ptr<VROIKJoint>> children;

    // Intermediary joints between this IKJoint and its child that should be
    // 'locked' and left out of the IK calculation.
    std::vector<std::shared_ptr<VROIKJoint>> lockedJoints;
    std::vector<VROMatrix4f> lockedJointLocalTransforms;
};

/*
 A chain containing a list of VROIKJoint in sequence, used for inverse kinematic calculations.
 */
struct VROIKChain {
    // The net length of all the bones in this chain.
    float totalLength;

    // A list of all VROIKJoints in this chain. Joints are placed in
    // order from the root to the leaf.
    std::vector<std::shared_ptr<VROIKJoint>> chainJoints;

    // A list of all intermediary bone lengths connecting the joints within this chain.
    std::vector<float> boneLengths;

    // Pointers to parent and child chains.
    std::shared_ptr<VROIKChain> parentChain;
    std::vector<std::shared_ptr<VROIKChain>> childChains;

    // True if an IK calculation pass has been performed on this chain.
    bool processed;
};

/*
 The VROIKRig contains a tree of VROIKJoints representing the node or skeletal hierarchy on which
 to apply inverse kinematic calculations and constraints driven by end effectors.
 */
class VROIKRig {
public:
    // Constructor used for creating IK Rigs out of VRONode trees.
    VROIKRig(std::shared_ptr<VRONode> root,
             std::map<std::string, std::shared_ptr<VRONode>> endAffectors);

    // Constructor used for creating IK Rigs out of the 3D model's skeleton.
    VROIKRig(std::shared_ptr<VROSkeleton> skeleton,
                       std::map<std::string, int> endEffectorBoneIndexMap);
    ~VROIKRig();

    /*
     Sets the position for an effector in world space.
     */
    void setPositionForEffector(std::string effectorId, VROVector3f pos);

    /*
     Called during a render pass to perform a full IK calculation on this rig and to
     sync the results back to the node or skeletal hierarchical tree.
     */
    void processRig();

private:
    /*
     The root IKJoint of this rig
     */
    std::shared_ptr<VROIKJoint> _rootJoint;

    /*
     A vec of all IK Joints in this rig
     */
    std::vector<std::shared_ptr<VROIKJoint>> _allKnownIKJoints;

    /*
     Map of keys to IK joint effectors in this rig
     */
    std::map<std::string, std::shared_ptr<VROIKJoint>> _keyToEffectorMap;
    std::map<std::shared_ptr<VROIKJoint>, std::string> _effectorTokeyMap;

    /*
     Map representing the desired world positions of IK joint effectors by their keys
     */
    std::map<std::string, VROVector3f> _effectorDesiredPositionMap;

    /*
     True if we need to initialize / re-invalidate the joint structure of this rig
     */
    bool _initializeRig;

    /*
     True if we have already processed this rig with the latest set map of effector positions
     */
    bool _processedNewEffectorPositions;

    /*
     A vec of all root IK chains in this rig
     */
    std::vector<std::shared_ptr<VROIKChain>> _rootChains;

    /*
     A map of VROIKJoint Ids mapped to VRORigChains representing the end
     effectors for this rig.
     */
    std::map<int, std::shared_ptr<VROIKChain>> _endEffectorIdToChains;

    /*
     A map of end effector VROIKJoint Ids and its local rotational transform.
     */
    std::map<int, VROMatrix4f> _endEffectorIdLocalRotation;

    /*
     A vec of all known chains of this rig.
     */
    std::vector<std::shared_ptr<VROIKChain>> _allKnownChains;

    /*
     A reference to the skeleton used to construct this IKRig, if any.
     */
    std::shared_ptr<VROSkeleton> _skeleton;
    VROMatrix4f _modelRootToRootJoint;

    /*
     Initializes / constructs the IKJoint and IKChain structure of this rig.
     */
    void initializeRig();

    /*
     Given the end effectors, construct the rig's IK tree by starting from each end effector
     node/bone and tracing them back towards the root effector node/bone. This will
     also help filter out redundant node subtrees that do not need to be apart of the
     kinematic calculation.
     */
    void createSkeletalRigFromNodeTree(std::shared_ptr<VRONode> currentNode);
    void createSkeletalRigFromSkeletalModel(int boneId);

    /*
     Iterates through the rig to bypass intermediary IKjoints in between joint effectors from
     being computed, thereby effectively "locking" them in place.
     */
    void flagLockedJoints(std::shared_ptr<VROIKJoint> referenceJoint,
                          std::shared_ptr<VROIKJoint> currentJoint);

    /*
     Returns the local transform of the given reference joint.
     */
    VROMatrix4f getJointLocalTransform(std::shared_ptr<VROIKJoint> referenceJoint);

    /*
     Removes the given IKJoint from _allKnownIKJoints and thus from being processed in the
     IKRig as a part of the FABRIK computation.
     */
    void detachIKJoint(std::shared_ptr<VROIKJoint> joint);

    /*
     Construct a tree of VROIKChains, each containing a sequence of IKJoints in this rig. This is
     is done by creating a new chain for every branching point in the IKJoint tree, starting
     from the root of the tree.
     */
    void formChains(std::shared_ptr<VROIKJoint> branchNodeStart,
                    std::shared_ptr<VROIKJoint> currentNode,
                    std::shared_ptr<VROIKChain> &currentChain);
    void formChainDependencies(std::shared_ptr<VROIKChain> &currentChain);

    /*
     Main kinematic functions for performing a FABRIK pass.
     */
    void processInverseKinematics();
    void processChainTreeTowardsRoot(std::shared_ptr<VROIKChain> &chain);
    void processChainTreeTowardsEffectors(std::shared_ptr<VROIKChain> &chain);
    void processFABRIKChainNode(std::shared_ptr<VROIKChain> &chain, bool reverse);
    bool hasEffectorsMetTarget();

    /*
     FUnctions for syncing the result of FABRIK calculations back into node / bone transforms.
     */
    void syncResultPositionOnly(std::shared_ptr<VROIKJoint> jointNode);
    void syncResultRotationOnly(std::shared_ptr<VROIKJoint> jointNode);
    void syncResultSkeleton(std::shared_ptr<VROIKJoint> jointNode);
    void syncLockedJoint(std::shared_ptr<VROIKJoint> jointNode, VROMatrix4f parentTrans);
};

#endif /* VROIKRig_h */
