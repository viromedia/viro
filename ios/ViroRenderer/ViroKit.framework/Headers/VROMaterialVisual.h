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
    void setColor(VROVector4f contents);
    void setTexture(std::shared_ptr<VROTexture> texture);
    
    VROTextureType getTextureType() const {
        if (_contentsTexture) {
            return _contentsTexture->getType();
        }
        else {
            return VROTextureType::None;
        }
    }
    
    VROVector4f getColor() const {
        return _contentsColor;
    }
    
    std::shared_ptr<VROTexture> getTexture() const {
        if (_contentsTexture) {
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
     The color component of the visual.
     */
    VROVector4f _contentsColor;
    
    /*
     The texture component of the visual.
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
