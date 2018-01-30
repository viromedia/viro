//
//  VROMaterial.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/17/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROMaterial_h
#define VROMaterial_h

#include <memory>
#include "VROMaterialVisual.h"
#include "VROAnimatable.h"
#include "VROStringUtil.h"

enum class VROFace {
    Front,
    Back,
    FrontAndBack
};

enum class VROCullMode {
    Back,
    Front,
    None
};

enum class VROBlendMode {
    None,
    Alpha,
    Add,
    Multiply,   // Note: Unimplemented mode
    Subtract,   // Note: Unimplemented mode
    Screen,     // Note: Unimplemented mode
    Replace     // Note: Unimplemented mode
};

enum class VROTransparencyMode {
    AOne,
    RGBZero
};

enum class VROLightingModel {
    Phong,
    Blinn,
    Lambert,
    Constant,
    PhysicallyBased
};

class VROLight;
class VRODriver;
class VROSortKey;
class VROMaterialSubstrate;
class VROShaderModifier;

/*
 Manages the lighting and shading attributes associated with the surface of a geometry that
 define its appearance when rendered. When you create a material, you define a collection of
 visual attributes and their options, which you can then reuse for multiple geometries 
 in a scene.
 */
class VROMaterial : public VROAnimatable {
    
public:
    
    VROMaterial();
    virtual ~VROMaterial();
    
    /*
     Delete any rendering resources. Invoked prior to destruction, on the
     rendering thread.
     */
    void deleteGL();
    
    /*
     Copy constructor for this material. Texture contents use shared references.
     */
    VROMaterial(std::shared_ptr<VROMaterial> material);
    
    /*
     Copy function, to write over all properties of this material with the
     properties of the given material.
     */
    void copyFrom(std::shared_ptr<VROMaterial> material);
    
    /*
     Set a name for this material. No functionality, only used for 
     debugging.
     */
    void setName(std::string name) {
        _name = name;
    }
    std::string getName() const {
        return _name;
    }
    
    uint32_t getMaterialId() const {
        return _materialId;
    }
    
    /*
     Bind shader and properties. These must be called in order: material properties
     cannot be bound until the shader is bound.
     
     Lights are passed into bindShader because the shader used by a material
     is a function both of that material's properties and of the desired lighting
     configuration.
     */
    void bindShader(int lightsHash,
                    const std::vector<std::shared_ptr<VROLight>> &lights,
                    const VRORenderContext &context,
                    std::shared_ptr<VRODriver> &driver);
    void bindProperties(std::shared_ptr<VRODriver> &driver);

    VROMaterialVisual &getDiffuse() const {
        return *_diffuse;
    }
    VROMaterialVisual &getRoughness() const {
        return *_roughness;
    }
    VROMaterialVisual &getMetalness() const {
        return *_metalness;
    }
    VROMaterialVisual &getSpecular() const {
        return *_specular;
    }
    VROMaterialVisual &getNormal() const {
        return *_normal;
    }
    VROMaterialVisual &getReflective() const {
        return *_reflective;
    }
    VROMaterialVisual &getEmission() const {
        return *_emission;
    }
    VROMaterialVisual &getMultiply() const {
        return *_multiply;
    }
    VROMaterialVisual &getAmbientOcclusion() const {
        return *_ambientOcclusion;
    }
    VROMaterialVisual &getSelfIllumination() const {
        return *_selfIllumination;
    }
    
    void setShininess(float shininess);
    float getShininess() const {
        return _shininess;
    }
    
    void setFresnelExponent(float fresnelExponent);
    float getFresnelExponent() const {
        return _fresnelExponent;
    }
    
    void setTransparency(float transparency);
    float getTransparency() const {
        return _transparency;
    }
    
    void setTransparencyMode(VROTransparencyMode mode) {
        _transparencyMode = mode;
    }
    VROTransparencyMode getTransparencyMode() const {
        return _transparencyMode;
    }
    
    void setLightingModel(VROLightingModel model) {
        _lightingModel = model;
    }
    VROLightingModel getLightingModel() const {
        return _lightingModel;
    }
    
    void setCullMode(VROCullMode cullMode) {
        _cullMode = cullMode;
    }
    VROCullMode getCullMode() const {
        return _cullMode;
    }
    
    bool isLitPerPixel() const {
        return _litPerPixel;
    }
    
    VROBlendMode getBlendMode() const {
        return _blendMode;
    }
    void setBlendMode(VROBlendMode mode){
        _blendMode = mode;
    }

    bool getWritesToDepthBuffer() const {
        return _writesToDepthBuffer;
    }
    bool getReadsFromDepthBuffer() const {
        return _readsFromDepthBuffer;
    }
    
    void setWritesToDepthBuffer(bool writesToDepthBuffer) {
        _writesToDepthBuffer = writesToDepthBuffer;
        updateSubstrate();
    }
    void setReadsFromDepthBuffer(bool readsFromDepthBuffer) {
        _readsFromDepthBuffer = readsFromDepthBuffer;
        updateSubstrate();
    }
    
    void setBloomThreshold(float threshold) {
        bool needsSubstrateUpdate = (_bloomThreshold >= 0 && threshold < 0) || (_bloomThreshold < 0 && threshold >= 0);
        _bloomThreshold = threshold;
        
        if (needsSubstrateUpdate) {
            updateSubstrate();
        }
    }
    float getBloomThreshold() const {
        return _bloomThreshold;
    }
    bool isBloomEnabled() const {
        return _bloomThreshold >= 0;
    }

    void setReceivesShadows(bool receivesShadows) {
        _receivesShadows = receivesShadows;
        updateSubstrate();
    }
    bool getReceivesShadows() const {
        return _receivesShadows;
    }

    void addShaderModifier(std::shared_ptr<VROShaderModifier> modifier);
    void removeShaderModifier(std::shared_ptr<VROShaderModifier> modifier);
    bool hasShaderModifier(std::shared_ptr<VROShaderModifier> modifier);
    const std::vector<std::shared_ptr<VROShaderModifier>> &getShaderModifiers() const {
        return _shaderModifiers;
    }
    void removeAllShaderModifiers() {
        _shaderModifiers.clear();
    }
    
    /*
     Make a snapshot of this material and cross-fade that snapshot out,
     bringing in the current material. Used to animate material changes.
     No effect if there is no active animation transaction.
     */
    void fadeSnapshot();
    std::shared_ptr<VROMaterial> getOutgoing() const {
        return _outgoing;
    }
    
    /*
     Check if the material has been updated since the last substrate was
     created.
     */
    bool isUpdated() {
        return _substrate == nullptr;
    }
    
    /*
     Force the substrate of this material to update on the next render cycle.
     */
    void updateSubstrate();
    
    /*
     Faster than updateSubstrate(), used only when the textures of this material are
     updated.
     */
    void updateSubstrateTextures();
    
    /*
     Get the representation of this material in the underlying graphics 
     technology.
     */
    VROMaterialSubstrate *const getSubstrate(std::shared_ptr<VRODriver> &driver);
    
    /*
     Update the given sort key with fields from this material, if the given
     lights are used in the render.
     */
    void updateSortKey(VROSortKey &key, const std::vector<std::shared_ptr<VROLight>> &lights,
                       const VRORenderContext &context,
                       std::shared_ptr<VRODriver> &driver);

    /*
     Returns a VROBlendMode for the given string. If no matching blend modes were found,
     VROBlendMode::None is returned.
     */
    static VROBlendMode getBlendModeFromString(std::string strType) {
        if (VROStringUtil::strcmpinsensitive(strType, "Alpha")){
            return VROBlendMode::Alpha;
        } else if (VROStringUtil::strcmpinsensitive(strType, "Add")){
            return VROBlendMode::Add;
        } else if (VROStringUtil::strcmpinsensitive(strType, "Multiply")){
            return VROBlendMode::Multiply;
        }
        return VROBlendMode::None;
    }

private:
    
    uint32_t _materialId;
    
    /*
     Properties used for PBR.
     */
    VROMaterialVisual *_diffuse;
    VROMaterialVisual *_roughness;
    VROMaterialVisual *_metalness;
    VROMaterialVisual *_ambientOcclusion;
    
    /*
     Properties used for standard lighting.
     */
    VROMaterialVisual *_specular;
    VROMaterialVisual *_reflective;
    
    /*
     Properties used for special effects.
     */
    VROMaterialVisual *_normal;
    VROMaterialVisual *_emission;         // Unsupported (TODO VIRO-1188)
    VROMaterialVisual *_multiply;         // Unsupported (TODO VIRO-1190)
    
    /*
     Currently unsupported. This will be used to override the Scene's overall environment 
     map with a material-specific environment map.
     */
    VROMaterialVisual *_selfIllumination;
    
    /*
     User-provided name of the material.
     */
    std::string _name;
    
    /*
     The sharpness of specular highlights.
     */
    float _shininess;
    
    /*
     Factor affecting material reflectivity.
     */
    float _fresnelExponent;
    
    /*
     Uniform transparency of the material.
     */
    float _transparency;
    
    /*
     The mode used to calculate transparency.
     */
    VROTransparencyMode _transparencyMode;
    
    /*
     The lighting model to use to compute the interaction between
     the lights in the scene and this material's visual properties.
     */
    VROLightingModel _lightingModel;
    
    /*
     True means use per-pixel lighting, false means use per-vertex lighting.
     */
    bool _litPerPixel;
    
    /*
     Set to cull back faces, front faces, or none.
     */
    VROCullMode _cullMode;
    
    /*
     Determines how pixel colors rendered using this material blend with 
     pixel colors already in the render target.
     */
    VROBlendMode _blendMode;
    
    /*
     Depth write and read settings.
     */
    bool _writesToDepthBuffer, _readsFromDepthBuffer;
    
    /*
     Version of this material that's being animated away. Populated with the current
     values of this material whenever this material is changed.
     */
    std::shared_ptr<VROMaterial> _outgoing;
    
    /*
     Modifiers to alter the shader code.
     */
    std::vector<std::shared_ptr<VROShaderModifier>> _shaderModifiers;
    
    /*
     If fragments of this material exceed this value, then those fragments will
     glow. If less than 0, bloom will be disabled. Defaults to -1.
     */
    float _bloomThreshold;

    /*
     True if this material receives shadows. Defaults to true.
     */
    bool _receivesShadows;
    
    /*
     Representation of this material in the underlying graphics hardware.
     */
    VROMaterialSubstrate *_substrate;
    
    void removeOutgoingMaterial();
    
};

#endif /* VROMaterial_h */
