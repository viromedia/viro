//
//  VROMaterial.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/17/15.
//  Copyright © 2015 Viro Media. All rights reserved.
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

#ifndef VROMaterial_h
#define VROMaterial_h

#include <memory>
#include <functional>
#include "VROMaterialVisual.h"
#include "VROAnimatable.h"
#include "VROStringUtil.h"
#include "VROThreadRestricted.h"
#include "VRODriver.h"

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
    Multiply,
    Subtract,
    Screen,
    PremultiplyAlpha,
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
class VROMaterial : public VROAnimatable, public VROThreadRestricted {
    
public:
    
    VROMaterial();
    virtual ~VROMaterial();

    /*
     Hydrate the material substrate and all textures. If this is not called, these resources
     will be automatically hydrated during the render-cycle.
     */
    void prewarm(std::shared_ptr<VRODriver> driver);
    
    /*
     Asynchronously upload the textures used by this material to the GPU, on the rendering
     thread as time permits. Each time a texture is uploaded, the given callback will be
     invoked.
     
     Returns the number of textures that were queued for upload. If a texture is *already*
     uploaded, that texture will not be returned in the count, and the callback will not
     be invoked for that texture.
     */
    int hydrateAsync(std::function<void()> callback,
                     std::shared_ptr<VRODriver> &driver);
    
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
     
     Returns false if the shader could not be bound.
     */
    bool bindShader(int lightsHash,
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
        // Update substrate not needed; pulled directly into uniform in VROMaterialShaderBinding
        return _shininess;
    }
    
    void setFresnelExponent(float fresnelExponent);
    float getFresnelExponent() const {
        return _fresnelExponent;
    }

    /*
     Transparency.
     */
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

    /*
     Lighting models.
     */
    void setLightingModel(VROLightingModel model) {
        _lightingModel = model;
        updateSubstrate();
    }
    VROLightingModel getLightingModel() const {
        return _lightingModel;
    }
    
    void setCullMode(VROCullMode cullMode) {
        // Update substrate not needed; bound in VROMaterial::bindProperties
        _cullMode = cullMode;
    }
    VROCullMode getCullMode() const {
        return _cullMode;
    }
    
    bool isLitPerPixel() const {
        return _litPerPixel;
    }

    /*
     Blend modes.
     */
    VROBlendMode getBlendMode() const {
        return _blendMode;
    }
    void setBlendMode(VROBlendMode mode) {
        // Update substrate not needed; bound in VROMaterial::bindProperties
        _blendMode = mode;
    }

    /*
     Depth writes.
     */
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
    
    /*
     Color writes.
     */
    VROColorMask getColorWriteMask(VROColorMask colorMask) const {
        return _colorWriteMask;
    }
    void setColorWriteMask(VROColorMask colorMask) {
        // Update substrate not needed; bound in VROMaterial::bindProperties
        _colorWriteMask = colorMask;
    }

    /*
     Bloom.
     */
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
    bool isBloomSupported() const {
        return _bloomThreshold >= 0;
    }

    /*
     Post Processing Mask.
     */
    void setPostProcessMask(bool mask) {
        _postProcessMask = mask;
    }
    bool getPostProcessMask() const {
        return _postProcessMask;
    }

    /*
     Shadows.
     */
    void setReceivesShadows(bool receivesShadows) {
        _receivesShadows = receivesShadows;
        updateSubstrate();
    }
    bool getReceivesShadows() const {
        return _receivesShadows;
    }

    void setCastsShadows(bool castsShadows) {
        _castsShadows = castsShadows;
        updateSubstrate();
    }
    bool getCastsShadows() const {
        return _castsShadows;
    }

    /*
     Chroma-key filtering.
     */
    void setChromaKeyFilteringEnabled(bool enabled);
    bool isChromaKeyFilteringEnabled() const {
        return _chromaKeyFilteringEnabled;
    }

    void setChromaKeyFilteringColor(VROVector3f color);
    VROVector3f getChromaKeyFilteringColor() const {
        return _chromaKeyFilteringColor;
    }
    
    /*
     Tone-mapping settings.
     */
    void setNeedsToneMapping(bool needsToneMapping);
    bool needsToneMapping() const {
        return _needsToneMapping;
    }

    /*
     Material rendering order; this should only be used to fix a rendering order between materials
     that are part of the same geometry. For cross-geometry rendering order, use
     VRONode::setRenderingOrder().
     */
    int getRenderingOrder() const {
        return _renderingOrder;
    }
    void setRenderingOrder(int renderingOrder) {
        _renderingOrder = renderingOrder;
    }

    /*
     Shader modifiers.
     */
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
     Return true if this material has an alpha channel associated with its diffuse color or
     texture.
     */
    bool hasDiffuseAlpha() const;

    /*
     Returns a VROBlendMode for the given string. If no matching blend modes were found,
     VROBlendMode::None is returned.
     */
    static VROBlendMode getBlendModeFromString(std::string strType) {
        if (VROStringUtil::strcmpinsensitive(strType, "Alpha")) {
            return VROBlendMode::Alpha;
        } else if (VROStringUtil::strcmpinsensitive(strType, "Add")) {
            return VROBlendMode::Add;
        } else if (VROStringUtil::strcmpinsensitive(strType, "Multiply")) {
            return VROBlendMode::Multiply;
        } else if (VROStringUtil::strcmpinsensitive(strType, "Subtract")) {
            return VROBlendMode::Subtract;
        } else if (VROStringUtil::strcmpinsensitive(strType, "Screen")) {
            return VROBlendMode::Screen;
        } else if (VROStringUtil::strcmpinsensitive(strType, "None")) {
            return VROBlendMode::None;
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
     Color mask settings. Materials will only write to color channels that are true
     in this bitfield. If all of these are set to off, then the material will not
     write color at all. This can be used to write only to the depth buffer to create
     transparent occlusion surfaces.
     */
    VROColorMask _colorWriteMask;
    
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
     True if this material is to be used for the creation of a post processing mask.
     */
    bool _postProcessMask;

    /*
     True if this material receives shadows. Defaults to true.
     */
    bool _receivesShadows;

    /*
     True if surfaces using this material cast shadows. Note: due to a technical limitation,
     geometries either cast shadows as a whole (across all materials) or not at all. If
     _castsShadows is false for at least one material in a Node, then no materials in that
     Node will cast shadows.
     */
    bool _castsShadows;

    /*
     The chroma key filter to be applied to this material. If enabled, all pixels with color 'near'
     the chromaKeyFilteringColor will become transparent. This is useful for rendering videos with alpha.
    */
    bool _chromaKeyFilteringEnabled;
    VROVector3f _chromaKeyFilteringColor;
    
    /*
     If true, then surfaces of this material are need to be tone-mapped when rendering in HDR.
     Tone-mapping is the process of reducing a surface's color values from HDR to LDR (0,1).
     Because tone-mapping will distort, saturate, or desaturate colors, it's sometimes useful
     to set some materials to not be tone-mapped: instead, these materials' colors will be
     linearly mapped from HDR to LDR. This is most useful when rendering text (which does not
     respond to light) or LDR backgrounds (which already contain accurate lighting
     information). You should not set this to false for any material that responds to light,
     especially in PBR.
    
     Defaults to true.
     */
    bool _needsToneMapping;

    /*
     The rendering order of this material, which determines when it is rendered in relation to
     other materials. See VROSortKey for where this falls within the hierarchy of renderinng sort
     concerns.
     */
    int _renderingOrder;
    
    /*
     Representation of this material in the underlying graphics hardware.
     */
    VROMaterialSubstrate *_substrate;
    
    void removeOutgoingMaterial();
    bool isHydrated();
    
};

#endif /* VROMaterial_h */
