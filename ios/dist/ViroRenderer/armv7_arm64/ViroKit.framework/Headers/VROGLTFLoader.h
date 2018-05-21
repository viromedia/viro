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
#include "VROGeometrySource.h"
#include "VROGeometryElement.h"
#include "VROMaterial.h"
#include "VROModelIOUtil.h"
#include "VROByteBuffer.h"

class VRONode;
class VROTexture;
class VROGeometry;
class VROSkinner;
class VROSkeleton;
class VROTaskQueue;
class VROSkeletalAnimation;
class VROKeyframeAnimation;

namespace tinygltf {
    class Model;
    class Scene;
    class Accessor;
    class Mesh;
    class Material;
    class Texture;
    class Parameter;
    class Primitive;
}

/*
 Enum representing the format of the data stored within the buffers of a GLTF model.
 Each GLTFType is associated with a value that represents the number of
 GLTFTypeComponent contained within this GTLFType. For example, GLTFType::VEC3 has 3
 GLTFTypeComponent components.
 */
enum class GLTFType {
    Scalar  = 1,
    Vec2    = 2,
    Vec3    = 3,
    Vec4    = 4,
    Mat2    = 4,
    Mat3    = 9,
    Mat4    = 10
};

/*
 Enum representing the byte size of a single GLTFTypeComponent contained within a GLTFType.
 */
enum class GLTFTypeComponent {
    Byte            = 1,
    UnsignedByte   = 1,
    Short           = 2,
    UnsignedShort  = 2,
    UnsignedInt    = 4,
    Float           = 4
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
    static bool processVertexElement(const tinygltf::Model &gModel, const tinygltf::Primitive &gPrimitive,
                                     std::vector<std::shared_ptr<VROGeometryElement>> &element);
    static bool processVertexAttributes(const tinygltf::Model &gModel, std::map<std::string, int> &gAttributes,
                                        std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                                        size_t geoElementIndex);
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

    /*
     As multiple mesh attributes may point to the same texture or data arrays when loading a
     GTLF model, we cache them here during loadGLTFFromResource(), and clear them out after.
     */
    static std::map<std::string, std::shared_ptr<VROData>> _dataCache;
    static std::map<std::string, std::shared_ptr<VROTexture>> _textureCache;
};

#endif /* VROGLTFLoader_h */
