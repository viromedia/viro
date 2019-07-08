//
//  VROGLTFLoader.h
//  ViroRenderer
//
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROGLTFLoader_h
#define VROGLTFLoader_h

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <functional>
#include <set>
#include "VROGeometrySource.h"
#include "VROGeometryElement.h"
#include "VROMaterial.h"
#include "VROModelIOUtil.h"
#include "VROByteBuffer.h"

class VROMorpher;
class VRONode;
class VROTexture;
class VROGeometry;
class VROSkinner;
class VROSkeleton;
class VROTaskQueue;
class VROSkeletalAnimation;
class VROKeyframeAnimation;
class VROKeyframeAnimationFrame;

namespace tinygltf {
    class Model;
    class Scene;
    class Accessor;
    class Mesh;
    class Material;
    class Texture;
    class Parameter;
    class Primitive;
    class Skin;
    class Animation;
    class AnimationSampler;
}

/*
 Enum representing the format of the data stored within the buffers of a GLTF model.
 Each GLTFType is associated with a value that represents the number of
 GLTFTypeComponent contained within this GTLFType. For example, GLTFType::VEC3 has 3
 GLTFTypeComponent components.
 */
enum class GLTFType {
    Scalar,
    Vec2,
    Vec3,
    Vec4,
    Mat2,
    Mat3,
    Mat4
};

/*
 Enum representing the byte size of a single GLTFTypeComponent contained within a GLTFType.
 */
enum class GLTFTypeComponent {
    Byte,
    UnsignedByte,
    Short,
    UnsignedShort,
    UnsignedInt,
    Float
};

/*
 Handles the loading of a GLTF model that is represented by a given .gltf or .glb file.
 TinyGltf is used to parse raw GTLF data into a tinygltf::Model format, after which we then
 use to construct a tree of VRONodes and VROGeometries representing the 3D model.
 */
class VROGLTFLoader {
public:
    static void loadGLTFFromResource(std::string gltfManifestFilePath,
                                     const std::map<std::string, std::string> overwriteResourceMap,
                                     VROResourceType resourceType,
                                     std::shared_ptr<VRONode> rootNode,
                                     bool isGLTFBinary,
                                     std::shared_ptr<VRODriver> driver,
                                     std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish = nullptr);

private:
    // Functions for processing basic components required for constructing a 3D Model in Viro.
    static bool processScene(const tinygltf::Model &gModel, std::shared_ptr<VRONode> rootNode, const tinygltf::Scene &gScene);
    static bool processNode(const tinygltf::Model &gModel, std::shared_ptr<VRONode> &sceneNode, int gNodeIndex);
    static bool processMesh(const tinygltf::Model &gModel, std::shared_ptr<VRONode> &node, const tinygltf::Mesh &gMesh);
    static bool processSkin(const tinygltf::Model &gModel, std::shared_ptr<VRONode> &node, int skinIndex);
    static bool processVertexElement(const tinygltf::Model &gModel, const tinygltf::Primitive &gPrimitive,
                                     std::vector<std::shared_ptr<VROGeometryElement>> &element);
    static bool processVertexAttributes(const tinygltf::Model &gModel, std::map<std::string, int> &gAttributes,
                                        std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                                        size_t geoElementIndex);
    static void processTangent(std::vector<std::shared_ptr<VROGeometryElement>> &elements,
                               std::vector<std::shared_ptr<VROGeometrySource>> &sources, size_t geoElementIndex);
    static void regenerateTangent(std::vector<VROVector3f> &posArray,
                                  std::vector<VROVector3f> &normArray,
                                  std::vector<VROVector3f> &texCoordArray,
                                  std::vector<int> &elementIndicesArray,
                                  std::vector<VROVector4f> &generatedTangents);
    static bool processMorphTargets(const tinygltf::Model &gModel,
                                    const tinygltf::Mesh &gMesh,
                                    const tinygltf::Primitive &gPrimitive,
                                    std::shared_ptr<VROMaterial> &material,
                                    std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                                    std::vector<std::shared_ptr<VROGeometryElement>> &elements,
                                    std::map<int, std::shared_ptr<VROMorpher>> &morphers);
    static std::string getMorphTargetName(const tinygltf::Model &gModel,
                                          const tinygltf::Primitive &gPrimtive, int targetIndex);

    static void injectGLTF(std::shared_ptr<VRONode> gltfNode, std::shared_ptr<VRONode> rootNode,
                           std::shared_ptr<VRODriver> driver, std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish);

    // Processing of GTLF Materials and Textures into VROMaterials and VROTextures
    static std::shared_ptr<VROMaterial> getMaterial(const tinygltf::Model &gModel, const tinygltf::Material &gMat);
    static std::shared_ptr<VROTexture> getTexture(const  tinygltf::Model &gModel, std::map<std::string, tinygltf::Parameter> gPropMap,
                                                  std::string targetedTextureName, bool srgb);
    static std::shared_ptr<VROTexture> getTexture(const tinygltf::Model &gModel, const tinygltf::Texture &texture, bool srgb);
    static void processPBR(const tinygltf::Model &gModel, std::shared_ptr<VROMaterial> &texture, const tinygltf::Material &gMat);

    // Conversion of GLTF Semantics to VRO Semantics
    static bool getPrimitiveType(int mode, VROGeometryPrimitiveType &type);
    static bool getComponentType(const tinygltf::Accessor &accessor, GLTFTypeComponent &typeComponent);
    static bool getComponent(const tinygltf::Accessor &accessor, GLTFType &type);
    static VROGeometrySourceSemantic getGeometryAttribute(std::string attributeName);
    static VROFilterMode getFilterMode(int mode);
    static VROWrapMode getWrappingMode(int mode);

    // Processing of Animation Data
    static bool processAnimations(const tinygltf::Model &gModel);
    static bool processAnimationKeyFrame(const tinygltf::Model &gModel,
                                         std::map<int, std::map<int, std::map<int, std::vector<int>>>> &gltfAnimatedNodes);
    static bool processAnimationChannels(const tinygltf::Model &gModel,
                                         const tinygltf::Animation &anim,
                                         std::vector<int> targetedChannels,
                                         std::shared_ptr<VROKeyframeAnimation> &animKeyFrameOut);
    static bool processRawChannelData(const tinygltf::Model &gModel,
                                      std::string channelProperty,
                                      int channelTarget,
                                      const tinygltf::AnimationSampler &gChannelSampler,
                                      std::vector<std::unique_ptr<VROKeyframeAnimationFrame>> &framesOut);
    static void processSkeletalAnimation(const tinygltf::Model &gModel,
                                         std::vector<std::pair<int,int>> &skeletalAnimToSkinPair);
    static void processSkeletalTransformsForFrame(int skin,
                                                  int animation,
                                                  int keyFrameIndex,
                                                  int currentJointIndex,
                                                  std::map<int, VROMatrix4f> &transforms);
    static bool processSkinner(const tinygltf::Model &gModel);
    static bool processSkinnerInverseBindData(const tinygltf::Model &gModel,
                                              const tinygltf::Skin &skin,
                                              std::vector<VROMatrix4f> &invBindTransformsOut);
    static void clearCachedData();

    /*
     As multiple mesh attributes may point to the same texture or data arrays when loading a
     GTLF model, we cache them here during loadGLTFFromResource(), and clear them out after.
     */
    static std::map<std::string, std::shared_ptr<VROData>> _dataCache;
    static std::map<std::string, std::shared_ptr<VROTexture>> _textureCache;

    /*
     Cached maps of skinner indexes to skeletal data, including both joints and affected node
     indexes. Note that in gLTF, a node can only have one skeletal root joint. These caches
     are cleared out after the parsing of a single gLTF model.
     */
    static std::map<int, std::shared_ptr<VROSkeleton>> _skinIndexToSkeleton;
    static std::map<int, std::map<int,int>> _skinIndexToJointNodeIndex;
    static std::map<int, std::map<int,std::vector<int>>> _skinIndexToJointChildJoints;
    static std::map<int, std::shared_ptr<VROSkinner>> _skinMap;

    /*
     Cached maps of nodeIndexes to it's corresponding animations. These caches are cleared
     out after the parsing of a single gLTF model. Note that _nodeKeyFrameAnims is of the form:
     <nodeIndex , <animationIndex, VROKeyframeAnimation>>> _nodeKeyframeAnims.
     */
    static std::map<int, std::map<int, std::shared_ptr<VROKeyframeAnimation>>> _nodeKeyFrameAnims;
    static std::map<int, std::vector<std::shared_ptr<VROSkeletalAnimation>>> _skinSkeletalAnims;

};

#endif /* VROGLTFLoader_h */
