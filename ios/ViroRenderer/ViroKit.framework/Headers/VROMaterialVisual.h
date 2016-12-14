//
//  VROMaterialVisual.hpp
//  ViroRenderer
//
//  Created by Raj Advani on 11/17/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROMaterialVisual_h
#define VROMaterialVisual_h

#include "VROVector4f.h"
#include "VROMatrix4f.h"
#include "VROTexture.h"
#include "VROImageUtil.h"
#include "VROAnimatable.h"
#include <vector>

enum class VROWrapMode {
    Clamp,
    Repeat,
    ClampToBorder,
    Mirror
};

enum class VROFilterMode {
    None,
    Nearest,
    Linear
};

enum class VROContentsType {
    Fixed = 1,
    Texture2D = 2,
    TextureCube = 4
};

class VROMaterial;

/*
 Used by the animation system, which requires shared pointers.
 */
class VROMaterialVisualHeartbeat : public VROAnimatable {
    
};

class VROMaterialVisual {
    
public:
    
    VROMaterialVisual(VROMaterial &material, int permissibleContentsMask) :
        _material(material),
        _permissibleContentsMask(permissibleContentsMask),
        _heartbeat(std::make_shared<VROMaterialVisualHeartbeat>()),
        _contentsType(VROContentsType::Fixed),
        _contentsColor({ 1.0, 1.0, 1.0, 1.0 }),
        _intensity(1.0),
        _wrapS(VROWrapMode::Clamp),
        _wrapT(VROWrapMode::Clamp),
        _minificationFilter(VROFilterMode::Linear),
        _magnificationFilter(VROFilterMode::Linear),
        _mipFilter(VROFilterMode::None),
        _borderColor( { 1.0, 1.0, 1.0, 1.0 })
    {}
    
    /*
     Copy constructor. Texture references are shared.
     */
    VROMaterialVisual(const VROMaterialVisual &visual);
    
    void clear();
    void setContents(VROVector4f contents);
    void setContents(std::shared_ptr<VROTexture> texture);
    void setContentsCube(std::shared_ptr<VROTexture> texture);
    
    VROContentsType getContentsType() const {
        return _contentsType;
    }
    
    VROVector4f getContentsColor() const {
        if (_contentsType == VROContentsType::Fixed) {
            return _contentsColor;
        }
        else {
            return { 1.0, 1.0, 1.0, 1.0 };
        }
    }
    
    std::shared_ptr<VROTexture> getContentsTexture() const {
        if (_contentsType == VROContentsType::Texture2D ||
            _contentsType == VROContentsType::TextureCube) {
            
            return _contentsTexture;
        }
        else {
            return getBlankTexture();
        }
    }
    
    void setIntensity(float intensity);
    float getIntensity() const {
        return _intensity;
    }
    
private:
    
    /*
     Parent material.
     */
    VROMaterial &_material;
    
    /*
     Bit mask of VROContentsType that indicates which contents types are 
     permissible to be set for this visual.
     */
    int _permissibleContentsMask;
    
    /*
     Shared pointer scoped to this material, for use by the animation system.
     */
    std::shared_ptr<VROMaterialVisualHeartbeat> _heartbeat;
    
    /*
     Indicates the content type for this visual.
     */
    VROContentsType _contentsType;
    
    /*
     If the visual is determined by a fixed color, _contentsColor is populated.
     */
    VROVector4f _contentsColor;
    
    /*
     If the visual is determined by a texture, this variable will be populated
     with the texture.
     */
    std::shared_ptr<VROTexture> _contentsTexture;
    
    /*
     Modulates the impact of this visual on the overall material appearance.
     */
    float _intensity;
    
    /*
     Transformation applied to the texture coordinates provided by the geometry object
     the material is attached to.
     */
    VROMatrix4f _contentsTransform;
    
    /*
     Standard texture mapping properties.
     */
    VROWrapMode _wrapS, _wrapT;
    VROFilterMode _minificationFilter, _magnificationFilter, _mipFilter;
    
    VROVector4f _borderColor;
    
};

#endif /* VROMaterialVisual_h */
