//
//  VROLight.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/7/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROLight_h
#define VROLight_h

#include <string>
#include <vector>
#include <atomic>
#include <memory>
#include "VROVector3f.h"
#include "VROVector4f.h"
#include "VROAnimatable.h"

static std::atomic_int sLightId;
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
    
    static uint32_t hashLights(const std::vector<std::shared_ptr<VROLight>> &lights);
    
    VROLight(VROLightType type) :
        _lightId(++sLightId),
        _type(type),
        _color({ 1.0, 1.0, 1.0 }),
        _updated(true),
        _attenuationStartDistance(2.0),
        _attenuationEndDistance(10.0),
        _attenuationFalloffExponent(2.0),
        _direction( { 0, 0, -1.0} ),
        _spotInnerAngle(0),
        _spotOuterAngle(45)
    {}
    
    ~VROLight()
    {}
    
    uint32_t getLightId() const {
        return _lightId;
    }
    
    VROLightType getType() const {
        return _type;
    }
    
    void setColor(VROVector3f color);
    VROVector3f getColor() const {
        return _color;
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
    
    void setTransformedPosition(VROVector3f position) {
        _transformedPosition = position;
    }
    VROVector3f getTransformedPosition() const {
        return _transformedPosition;
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
    
    /*
     Lights hold onto their UBOs so that they can propagate their
     updates to them. Each time a light is updated, it pushes the
     update to all of its parent UBOs.
     */
    void addUBO(std::shared_ptr<VROLightingUBO> ubo) {
        _ubos.push_back(ubo);
    }
    void propagateUpdates();
    
private:
    
    uint32_t _lightId;
    VROLightType _type;
    VROVector3f _color;
    
    std::string _name;
    bool _updated;
    
    /*
     Omni and Spot parameters.
     */
    VROVector3f _position;
    float _attenuationStartDistance;
    float _attenuationEndDistance;
    float _attenuationFalloffExponent;
    
    /*
     Diffuse parameters.
     */
    VROVector3f _direction;
    
    /*
     Spot parameters.
     */
    float _spotInnerAngle;
    float _spotOuterAngle;
    
    /*
     Internal. The position of the light after all its supernode
     transforms have been applied.
     */
    VROVector3f _transformedPosition;
    
    /*
     Weak refs to all UBOs that use this light. Needed so that when this
     light is updated, we can flag the UBO as a whole as requiring an 
     update.
     */
    std::vector<std::weak_ptr<VROLightingUBO>> _ubos;
    
};

#endif /* VROLight_h */
