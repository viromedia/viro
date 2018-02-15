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
    
    /*
     Update the textures used by this substrate. Invoked when the material's
     textures are updated, in lieu of regenerating the entire substrate.
     */
    virtual void updateTextures() = 0;
    
    /*
     Update the given sort key with the properties in this material, if the
     given lights are used.
     */
    virtual void updateSortKey(VROSortKey &key, const std::vector<std::shared_ptr<VROLight>> &lights,
                               const VRORenderContext &context,
                               std::shared_ptr<VRODriver> driver) = 0;
    
    /*
     Bind the shader used in this material to the active rendering context.
     This is kept independent of the bindProperties() function because shader changes
     are expensive, so we want to manage them independent of materials in the
     render loop.
     
     The shader used is a function both of the underlying material properties
     and of the desired lighting configuration.
     */
    virtual void bindShader(int lightsHash,
                            const std::vector<std::shared_ptr<VROLight>> &lights,
                            const VRORenderContext &context,
                            std::shared_ptr<VRODriver> &driver) = 0;
    
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
     Bind the properties of the view and projection to the active rendering
     context.
     */
    virtual void bindView(VROMatrix4f modelMatrix, VROMatrix4f viewMatrix,
                          VROMatrix4f projectionMatrix, VROMatrix4f normalMatrix,
                          VROVector3f cameraPosition, VROEyeType eyeType) = 0;
    
};

#endif /* VROMaterialSubstrate_h */
