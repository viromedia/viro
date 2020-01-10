//
//  VROMaterialVisual.hpp
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

#ifndef VROMaterialVisual_h
#define VROMaterialVisual_h

#include "VROVector4f.h"
#include "VROMatrix4f.h"
#include "VROTexture.h"
#include "VROImageUtil.h"
#include "VROAnimatable.h"
#include <vector>

class VROMaterial;

class VROMaterialVisual {
    
public:
    
    VROMaterialVisual(VROMaterial &material, int permissibleContentsMask) :
        _material(material),
        _permissibleContentsMask(permissibleContentsMask),
        _contentsColor({ 1.0, 1.0, 1.0, 1.0 }),
        _intensity(1.0)
    {}

    VROMaterialVisual(VROMaterial &material, int permissibleContentsMask, VROVector4f color) :
            _material(material),
            _permissibleContentsMask(permissibleContentsMask),
            _contentsColor(color),
            _intensity(1.0)
    {}
    
    /*
     Copy constructor; copy the properties of the given visual, but parent this visual
     with the the given material.
     */
    VROMaterialVisual(VROMaterial &material, const VROMaterialVisual &visual);
    
    /*
     Delete any rendering resources. Invoked prior to destruction, on the
     rendering thread.
     */
    void deleteGL();
    
    /*
     Copy from the given visual to this one. Does NOT copy the
     parent material, however.
     */
    void copyFrom(const VROMaterialVisual &visual);
    
    void clear();
    void setColor(VROVector4f contents);
    void setTexture(std::shared_ptr<VROTexture> texture);
    
    /*
     Used to replace a texture quickly without regenerating the
     entire substrate. If this is the first time adding a texture,
     then the substrate must be replaced. In this case we return
     true.
     */
    bool swapTexture(std::shared_ptr<VROTexture> texture);
    
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
            return getBlankTexture(VROTextureType::None);
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
    
};

#endif /* VROMaterialVisual_h */
