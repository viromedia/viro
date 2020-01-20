//
//  VROLight.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/7/15.
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

#ifndef VROLight_h
#define VROLight_h

#include <string>
#include <vector>
#include <atomic>
#include <memory>
#include <limits>
#include "VROVector3f.h"
#include "VROVector4f.h"
#include "VROMatrix4f.h"
#include "VROAnimatable.h"

static std::atomic_int sLightId;
class VROTexture;
class VROPencil;
class VROLightingUBO;

enum class VROLightType {
    Ambient,
    Directional,
    Omni,
    Spot
};

enum class VROShadowMode {
    Forward,
    Deferred,
    Modulated
};

class VROLight : public VROAnimatable {
    
public:
    
    /*
     Derive the RGB color (hue) from the given temperature in Kelvin.
     */
    static VROVector3f deriveRGBFromTemperature(float temperature);
    
    /*
     Convert the given RGB color from gamma space to linear space.
     */
    static VROVector3f convertGammaToLinear(VROVector3f color);
    
    /*
     Generate a hashcode from the given vector of lights.
     */
    static uint32_t hashLights(const std::vector<std::shared_ptr<VROLight>> &lights);
    
    VROLight(VROLightType type);
    ~VROLight() {}
    
    uint32_t getLightId() const {
        return _lightId;
    }
    
    VROLightType getType() const {
        return _type;
    }
    
#pragma mark - Light Properties
    
    void setColor(VROVector3f color);
    VROVector3f getColor() const {
        return _color;
    }
    
    void setIntensity(float intensity);
    float getIntensity() const {
        return _intensity;
    }
    
    void setTemperature(float temperature);
    float getTemperature() const {
        return _temperature;
    }
    VROVector3f getColorFromTemperature() const {
        return _colorFromTemperature;
    }
    
    void setName(std::string name) {
        this->_name = name;
    }
    std::string getName() const {
        return _name;
    }
    
    void setPosition(VROVector3f position);
    VROVector3f getPosition() const {
        return _position;
    }
    
    void setDirection(VROVector3f direction);
    VROVector3f getDirection() const {
        return _direction;
    }
    
    void setAttenuationStartDistance(float attenuationStartDistance);
    float getAttenuationStartDistance() const {
        return _attenuationStartDistance;
    }
    
    void setAttenuationEndDistance(float attenuationEndDistance);
    float getAttenuationEndDistance() const {
        return _attenuationEndDistance;
    }
    
    void setAttenuationFalloffExponent(float attenuationFalloffExponent);
    float getAttenuationFalloffExponent() const {
        return _attenuationFalloffExponent;
    }
    
    void setSpotInnerAngle(float spotInnerAngle);
    float getSpotInnerAngle() const {
        return _spotInnerAngle;
    }
    
    void setSpotOuterAngle(float spotOuterAngle);
    float getSpotOuterAngle() const {
        return _spotOuterAngle;
    }
    
    void setInfluenceBitMask(int influenceBitMask) {
        _influenceBitMask = influenceBitMask;
    }
    int getInfluenceBitMask() const {
        return _influenceBitMask;
    }
    
#pragma mark - Shadow Properties
    
    void setCastsShadow(bool castsShadow);
    bool getCastsShadow() const {
        return _castsShadow;
    }
    
    void setShadowOpacity(float shadowOpacity);
    float getShadowOpacity() const {
        return _shadowOpacity;
    }
    
    void setShadowMapSize(int shadowMapSize) {
        _shadowMapSize = shadowMapSize;
    }
    int getShadowMapSize() const {
        return _shadowMapSize;
    }
    
    void setShadowBias(float shadowBias) {
        _updatedFragmentData = true;
        _shadowBias = shadowBias;
    }
    float getShadowBias() const {
        return _shadowBias;
    }
    
    void setShadowOrthographicSize(float size) {
        _shadowOrthographicSize = size;
    }
    float getShadowOrthographicSize() const {
        return _shadowOrthographicSize;
    }

    void setShadowNearZ(float nearZ) {
        _shadowNearZ = nearZ;
    }
    float getShadowNearZ() const {
        return _shadowNearZ;
    }
    
    void setShadowFarZ(float farZ) {
        _shadowFarZ = farZ;
    }
    float getShadowFarZ() const {
        return _shadowFarZ;
    }
    
#pragma mark - Light Implementation
    
    /*
     Lights hold onto their UBOs so that they can propagate their
     updates to them. Each time a light is updated, it pushes the
     update to all of its parent UBOs.
     */
    void addUBO(std::shared_ptr<VROLightingUBO> ubo) {
        _ubos.push_back(ubo);
    }
    void propagateFragmentUpdates();
    void propagateVertexUpdates();
    
    void setTransformedPosition(VROVector3f position);
    VROVector3f getTransformedPosition() const {
        return _transformedPosition;
    }
    
    void setTransformedDirection(VROVector3f direction);
    VROVector3f getTransformedDirection() const {
        return _transformedDirection;
    }
    
#pragma mark - Shadow Implementation

    int getShadowMapIndex() const {
        return _shadowMapIndex;
    }
    void setShadowMapIndex(int shadowMapIndex) {
        _updatedFragmentData = true;
        _shadowMapIndex = shadowMapIndex;
    }
    
    VROMatrix4f getShadowViewMatrix() const {
        return _shadowViewMatrix;
    }
    VROMatrix4f getShadowProjectionMatrix() const {
        return _shadowProjectionMatrix;
    }
    
    void setShadowViewMatrix(VROMatrix4f shadowViewMatrix) {
        _updatedVertexData = true;
        _shadowViewMatrix = shadowViewMatrix;
    }
    void setShadowProjectionMatrix(VROMatrix4f shadowProjectionMatrix) {
        _updatedVertexData = true;
        _shadowProjectionMatrix = shadowProjectionMatrix;
    }
    
#pragma mark - Debugging
    
    void drawLightFrustum(std::shared_ptr<VROPencil> pencil);
    
private:
    
    uint32_t _lightId;
    VROLightType _type;
    
    /*
     RGB color of the light.
     */
    VROVector3f _color;
    
    /*
     Luminous flux of the light, ranging from 0 to 1000. Modulates the color of the
     light.
     */
    float _intensity;
    
    /*
     The temperature of the light, in Kelvin. Viro will derive a hue from this temperature
     and multiply it by _color. To model a physical light, you can leave _color set to
     (1.0, 1.0, 1.0) and set the temperature only. The default value is 6500K, which
     represents pure white light.
     */
    float _temperature;
    
    /*
     The color as derived from _temperature.
     */
    VROVector3f _colorFromTemperature;
    
    std::string _name;
    bool _updatedFragmentData;
    bool _updatedVertexData;
    
    /*
     The position of the light. For omnidirectional and spotlights, this represents
     the position from which the light emanates. For directional lights there is no
     position, but we still use this field to indicate where the directional light's
     shadow map should be centered.
     */
    VROVector3f _position;
    
    /*
     Attenuation parameters for omni and spot lights.
     */
    float _attenuationStartDistance;
    float _attenuationEndDistance;
    float _attenuationFalloffExponent;
    
    /*
     Diffuse parameters.
     */
    VROVector3f _direction;
    
    /*
     Spot parameters. The inner angle is the angle from edge to edge of
     the 'full strength' light cone. The lighting is at maximum intensity
     within this cone, and begins to attenuate outside of it.
     
     The outer angle is the angle from edge to edge of the "attenuated"
     light cone. The lighting declines in strength betewen the inner angle
     and outer angle. Outside of the outer angle the light attenuates to
     zero, resulting in no light.
     
     Both of these are specified in degrees.
     */
    float _spotInnerAngle;
    float _spotOuterAngle;
    
    /*
     Internal. The position and direction of the light after all its supernode
     transforms have been applied.
     */
    VROVector3f _transformedPosition;
    VROVector3f _transformedDirection;
    
    /*
     Weak refs to all UBOs that use this light. Needed so that when this
     light is updated, we can flag the UBO as a whole as requiring an 
     update.
     */
    std::vector<std::weak_ptr<VROLightingUBO>> _ubos;
    
    /*
     True if this light casts shadows.
     */
    bool _castsShadow;
    
    /*
     The opacity of the shadow. 1.0 implies a pitch black shadow.
     */
    float _shadowOpacity;
    
    /*
     The size of the depth map to use to render shadows. Larger sizes produce
     more detailed shadows at higher cost to rendering performance. Lower sizes
     are faster but result in pixelation at the edges.
     */
    int _shadowMapSize;
    
    /*
     The amount of bias to apply to the Z coordinate when performing the shadow
     depth comparison. This reduces shadow acne, but large biases can increase
     "peter panning".
     */
    float _shadowBias;
    
    /*
     This property only applies to directional lights, where an orthographic
     projection is used to render the shadow map. The orthographic size determines
     the width and height of the area, centered at _position, that should be rendered
     to the shadow map.
     
     A larger value means more of the scene will be shadowed, but
     at lower resolution.
     */
    float _shadowOrthographicSize;
    
    /*
     The near and far clipping planes to use when rendering shadows. Shadows are
     only cast by surfaces within these planes.
     */
    float _shadowNearZ, _shadowFarZ;
    
    /*
     The index into the shadow render target's texture array where we can find this
     light's shadow map.
     */
    int _shadowMapIndex;
    
    /*
     Bit mask that is ANDed with each node's lightReceivingBitMask and shadowCastingBitMask
     to determine what objects are illuminated by, and cast shadows from, this light.
     
     Default is 1.
     */
    int _influenceBitMask;
    
    /*
     The view and projection matrices used to transform any point in world
     space into its corresponding texcoord in the light's shadow depth map.
     */
    VROMatrix4f _shadowViewMatrix;
    VROMatrix4f _shadowProjectionMatrix;
    
};

#endif /* VROLight_h */
