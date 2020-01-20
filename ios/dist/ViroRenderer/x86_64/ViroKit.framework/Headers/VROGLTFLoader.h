//
//  VROGLTFLoader.h
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
class VROVertexBuffer;
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
    class Buffer;
    class BufferView;
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
    static bool processScene(const tinygltf::Model &gModel, std::shared_ptr<VRONode> rootNode, const tinygltf::Scene &gScene,
                             std::shared_ptr<VRODriver> driver);
    static bool processNode(const tinygltf::Model &gModel, std::shared_ptr<VRONode> &sceneNode, int gNodeIndex,
                            std::shared_ptr<VRODriver> driver);
    static bool processMesh(const tinygltf::Model &gModel, std::shared_ptr<VRONode> &node, const tinygltf::Mesh &gMesh,
                            std::shared_ptr<VRODriver> driver);
    static bool processSkin(const tinygltf::Model &gModel, std::shared_ptr<VRONode> &node, int skinIndex);
    static bool processVertexElement(const tinygltf::Model &gModel, const tinygltf::Primitive &gPrimitive,
                                     std::vector<std::shared_ptr<VROGeometryElement>> &element);
    static bool processVertexAttributes(const tinygltf::Model &gModel, std::map<std::string, int> &gAttributes,
                                        std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                                        size_t geoElementIndex,
                                        std::shared_ptr<VRODriver> driver);
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
                                    std::map<int, std::shared_ptr<VROMorpher>> &morphers,
                                    std::shared_ptr<VRODriver> driver);
    static std::string getMorphTargetName(const tinygltf::Model &gModel,
                                          const tinygltf::Primitive &gPrimtive, int targetIndex);

    static void injectGLTF(std::shared_ptr<VRONode> gltfNode, std::shared_ptr<VRONode> rootNode,
                           std::shared_ptr<VRODriver> driver, std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish);
    
    static std::shared_ptr<VROGeometrySource> buildGeometrySource(VROGeometrySourceSemantic attributeType,
                                                                  GLTFType gType,
                                                                  GLTFTypeComponent gTypeComponent,
                                                                  const tinygltf::Accessor &gAttributeAccesor,
                                                                  const tinygltf::BufferView &gIndiceBufferView,
                                                                  std::shared_ptr<VROVertexBuffer> vbo);
    static std::shared_ptr<VROGeometrySource> buildBoneWeightSource(GLTFType gType,
                                                                    GLTFTypeComponent gTypeComponent,
                                                                    const tinygltf::Accessor &gAttributeAccesor,
                                                                    const tinygltf::BufferView &gIndiceBufferView,
                                                                    const tinygltf::Buffer &gbuffer);

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
    static bool processKeyFrameAnimations(const tinygltf::Model &gModel,
                                         std::map<int, std::map<int, std::vector<int>>> &gltfAnimatedNodes);
    static void flattenSkeletalKeyframeAnimations(
            std::map<int, std::pair<int, std::vector<int>>> &skeletalAnimToNodeSkinPair);
    static std::shared_ptr<VROKeyframeAnimation> convertChannelToKeyFrameAnimation(
                                                  const tinygltf::Model &gModel,
                                                  const tinygltf::Animation &anim,
                                                  int targetedChannel);
    static bool processRawChannelData(const tinygltf::Model &gModel,
                                      std::string channelProperty,
                                      int channelTarget,
                                      const tinygltf::AnimationSampler &gChannelSampler,
                                      std::vector<std::unique_ptr<VROKeyframeAnimationFrame>> &framesOut);
    static bool processSkeletalAnimation(const tinygltf::Model &gModel,
                                         std::map<int, std::pair<int, std::vector<int>>> &skeletalAnimToSkinToNodeMap);
    static bool processSkeletalTransformsForFrame(const tinygltf::Model &gModel,
                                                  int skin,
                                                  int animation,
                                                  int subAnimPropertyIndex,
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
    static std::map<std::string, std::shared_ptr<VROVertexBuffer>> _dataCache;
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
    static std::map<int, int> _skinIndexToSkeletonRootJoint;

    /*
     Cached maps of nodeIndexes to it's corresponding animations. These caches are cleared
     out after the parsing of a single gLTF model. Note that _nodeKeyFrameAnims is of the form:
     <nodeIndex , <animationIndex, VROKeyframeAnimation>>> _nodeKeyframeAnims.
     */
    static std::map<int, std::map<int, std::vector<std::shared_ptr<VROKeyframeAnimation>>>> _nodeKeyFrameAnims;
    static std::map<int, std::vector<std::shared_ptr<VROSkeletalAnimation>>> _skinSkeletalAnims;

    /*
     Returns the local transform of the node index retried from the gltf model.
     */
    static VROMatrix4f getTransformOfNode(const tinygltf::Model &gModel, int nodeIndex);
};

#endif /* VROGLTFLoader_h */
