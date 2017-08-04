//
//  VROMaterialSubstrate.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/29/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROMaterialSubstrate_h
#define VROMaterialSubstrate_h

#include <stdio.h>
#include <vector>
#include <memory>

class VROSortKey;
class VROLight;
class VRORenderContext;
class VRODriver;
class VROGeometry;
class VROVector3f;
class VROMatrix4f;
enum class VROEyeType;

class VROMaterialSubstrate {
public:
    virtual ~VROMaterialSubstrate() {}
    virtual void updateSortKey(VROSortKey &key) const = 0;
    
    /*
     Bind the shader used in this material to the active rendering context.
     This is kept independent of the bind() function because shader changes
     are expensive, so we want to manage them independent of materials in the
     render loop.
     */
    virtual void bindShader() = 0;
    
    /*
     Bind the properties of this material to the active rendering context.
     These properties should be node and geometry independent. The shader
     should always be bound first (via bindShader()).
     */
    virtual void bindProperties() = 0;
    
    /*
     Bind the properties of the given geometry to the active rendering context.
     These are material properties (e.g. shader uniforms) that are dependent
     on properties of the geometry.
     */
    virtual void bindGeometry(float opacity, const VROGeometry &geometry) = 0;
    
    /*
     Bind the properties of the given lights to the active rendering context.
     */
    virtual void bindLights(int lightsHash,
                            const std::vector<std::shared_ptr<VROLight>> &lights,
                            const VRORenderContext &context,
                            std::shared_ptr<VRODriver> &driver) = 0;
    
    /*
     Bind the properties of the view and projection to the active rendering
     context.
     */
    virtual void bindView(VROMatrix4f transform, VROMatrix4f modelview,
                          VROMatrix4f projectionMatrix, VROMatrix4f normalMatrix,
                          VROVector3f cameraPosition, VROEyeType eyeType) = 0;
    
};

#endif /* VROMaterialSubstrate_h */
