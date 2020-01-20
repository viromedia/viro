//
//  VRODriver.h
//  ViroRenderer
//
//  Created by Raj Advani on 4/21/16.
//  Copyright © 2016 Viro Media. All rights reserved.
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

#ifndef VRODriver_h
#define VRODriver_h

#include <vector>
#include "VRODefines.h"
#include "VROSoundData.h"

class VROGeometry;
class VROMaterial;
class VROGeometrySubstrate;
class VROMaterialSubstrate;
class VROTextureSubstrate;
class VROVertexBuffer;
class VROData;
class VROImage;
class VROVideoTextureCache;
class VROSound;
class VROAudioPlayer;
class VROTypefaceCollection;
class VROFrameTimer;
class VRORenderTarget;
class VRORenderContext;
class VROShaderProgram;
class VROImagePostProcess;
class VROFrameScheduler;

enum class VROSoundType;
enum class VROTextureType;
enum class VROTextureFormat;
enum class VROTextureInternalFormat;
enum class VROWrapMode;
enum class VROFilterMode;
enum class VROMipmapMode;
enum class VRORenderTargetType;
enum class VROFace;
enum class VROCullMode;
enum class VROBlendMode;
enum class VROResourceType;
enum class VROFontStyle;
enum class VROFontWeight;

enum VROColorMask {
    VROColorMaskNone  = 0,
    VROColorMaskRed   = 1,
    VROColorMaskGreen = 1 << 1,
    VROColorMaskBlue  = 1 << 2,
    VROColorMaskAlpha = 1 << 3,
    VROColorMaskAll   = (1) | (1 << 1) | (1 << 2) | (1 << 3)
};

/*
 The type of the GPU can be used to modify rendering for compatibility with older
 GPUs, or it can be used to gate access to advanced rendering features that only
 work with some subset of GPUs.
 */
enum class VROGPUType {
    /*
     Covers properly functioning GPUs.
     */
    Normal,

    /*
     This type is used for GPUs that supposedly support OpenGL 3.0 but can't handle
     simple 3D graphics without serious errors. This is primarily used for the Adreno 330
     and earlier, which have issues with structs in GLSL programs. The inclusion of structs can
     cause textures to corrupt (EGL textures gain a purple tint), and shaders to sporadically fail
     compilation.

     Since these GPUs are generally legacy and can't support AR and VR, instead of writing
     an entirely new set of shaders to support them, we instead force these GPUs to always
     use a simple constant shader, and modify the expand the Surface struct into separate
     variables.
     */
    Adreno330OrOlder,

    /*
     This type covers Mali GPUs, which fail to report that they've provisioned an sRGB framebuffer
     even when they have. We therefore disable software gamma correction on these devices.
     */
    Mali
};

/*
 The color mode used when rendering.

 NonLinear: textures are sampled in RGB (gamma-corrected) space, shader operations
            are performed in this non-linear space, and rendered straight to the frame
            buffer without conversion. In other words, inputs are gamma corrected and
            output is gamma corrected. Less color accuracy.

 Linear:    textures are sampled as sRGB, meaning they are converted to a linear
            colorspace by the underlying hardware. Colors remain in linear color space
            until the *final* framebuffer render, at which point they are gamma
            corrected by the hardware (by rendering into an sRGB-configured framebuffer)

 LinearSoftware: textures are sampled as sRGB, as with Linear, but the final framebuffer
                 is gamma-corrected via shader code. This is typically used in situations
                 where we are unable to allocate an sRGB framebuffer for the final render.
 */
enum class VROColorRenderingMode {
    NonLinear,
    Linear,
    LinearSoftware,
};

/*
 Operations to perform when unbinding a previous bound render target.
 
 None: Nothing is done to the previously bound target.
 
 Invalidate: Invalidate the previous target, which tells the driver it does not need
             to copy the contents of the target back into shared memory. Invalidating
             the previous target will make the rendered contents of the previous target
             inaccessible (discarding their contents), so blitColor and blitStencil will
             no longer work. However, it will not impact color attachments that were
             rendered to a texture.
 
 CopyStencilAndInvalidate: Copy the stencil buffer from the previously bound target to
                           the new target, then invalidate the previously bound target.
 */
enum class VRORenderTargetUnbindOp {
    None,
    Invalidate,
    CopyStencilAndInvalidate,
};

/*
 The driver is used to interface with the rendering subsystem (OpenGL,
 Metal, etc.).
 */
class VRODriver {
    
public:

    VRODriver();
    virtual ~VRODriver();
    
    /*
     Provides the driver an opportunity to update any sub-components
     with per-frame state. The willRenderFrame method is invoked just
     prior to rendering, and didRenderFrame is invoked afterward. The
     VROFrameTimer can be used by the driver to timebox its tasks so
     as not to overrun frame time.
     */
    virtual void willRenderFrame(const VRORenderContext &context) = 0;
    virtual void didRenderFrame(const VROFrameTimer &timer, const VRORenderContext &context) = 0;
    
    /*
     Provides the driver an opportunity to reset state between rendering
     different eyes within a frame.
     */
    virtual void willRenderEye(const VRORenderContext &context) = 0;
    virtual void didRenderEye(const VRORenderContext &context) = 0;
    
    /*
     Invoked when the renderer is paused and resumed.
     */
    virtual void pause() = 0;
    virtual void resume() = 0;

    /*
     Read the GPU type, which may be necessary to support old GPUs that are not to
     specification, or to control whether or not to use advanced features only supported
     by a subset of GPUs.
     */
    virtual void readGPUType() = 0;

    /*
     Get the GPU type, after it has been read by the system.
     */
    virtual VROGPUType getGPUType() = 0;
    
    /*
     Read the ID of the display's framebuffer. May not be required on all
     platforms. This way we're able to re-bind to the display during a multi-pass
     alogrithm.
     */
    virtual void readDisplayFramebuffer() = 0;
    
    /*
     The driver controls context-wide state change. The implementation
     should store a CPU copy of current state to avoid sending unnecessary
     instructions to the GPU.
     */
    virtual void setActiveTextureUnit(int unit) = 0;
    virtual void bindTexture(int target, int texture) = 0;
    virtual void bindTexture(int unit, int target, int texture) = 0;
    virtual void setDepthWritingEnabled(bool enabled) = 0;
    virtual void setDepthReadingEnabled(bool enabled) = 0;
    virtual void setStencilTestEnabled(bool enabled) = 0;
    virtual void setCullMode(VROCullMode cullMode) = 0;
    virtual void setRenderTargetColorWritingMask(VROColorMask mask) = 0;
    virtual void setMaterialColorWritingMask(VROColorMask mask) = 0;
    virtual void bindShader(std::shared_ptr<VROShaderProgram> program) = 0;
    virtual void unbindShader() = 0;
    
    /*
     Bind the given render target, and perform the given operation when unbinding
     the previous target. Returns true if the new target was bound. Returns false
     if the designated target was *already* bound.
     */
    virtual bool bindRenderTarget(std::shared_ptr<VRORenderTarget> target, VRORenderTargetUnbindOp unbindOp) = 0;
    virtual void unbindRenderTarget() = 0;
    
    /*
     Get the render target that is currently bound. Returns nullptr if no
     target is bound.
     */
    virtual std::shared_ptr<VRORenderTarget> getRenderTarget() = 0;
    
    /*
     Indicates how we handle linear rendering and gamma-correction for this device
     and platform. Features like HDR are only enabled in linear space.
     */
    virtual VROColorRenderingMode getColorRenderingMode() = 0;
    
    /*
     Sets a flag indicating whether a software gamma correction pass is currently
     being used by the choreographer.
     */
    virtual void setHasSoftwareGammaPass(bool softwareGamma) = 0;
    virtual bool hasSoftwareGammaPass() const = 0;
    
    /*
     Returns true if linear rendering is enabled for this device. This only returns true
     if:
     
     1. We have a hardware sRGB buffer, or
     2. We are using software gamma correction (which requires that HDR is enabled).
     */
    bool isLinearRenderingEnabled() {
        return getColorRenderingMode() == VROColorRenderingMode::Linear ||
              (getColorRenderingMode() == VROColorRenderingMode::LinearSoftware && hasSoftwareGammaPass());
    }
    
    /*
     Return true if bloom rendering is enabled. If so, materials that exceed their
     bloom threshold will glow.
     */
    virtual bool isBloomSupported() = 0;
    
    virtual VROGeometrySubstrate *newGeometrySubstrate(const VROGeometry &geometry) = 0;
    virtual VROMaterialSubstrate *newMaterialSubstrate(VROMaterial &material) = 0;
    virtual VROTextureSubstrate *newTextureSubstrate(VROTextureType type,
                                                     VROTextureFormat format,
                                                     VROTextureInternalFormat internalFormat, bool sRGB,
                                                     VROMipmapMode mipmapMode,
                                                     std::vector<std::shared_ptr<VROData>> &data,
                                                     int width, int height, std::vector<uint32_t> mipSizes,
                                                     VROWrapMode wrapS, VROWrapMode wrapT,
                                                     VROFilterMode minFilter, VROFilterMode magFilter, VROFilterMode mipFilter) = 0;
    virtual std::shared_ptr<VRORenderTarget> newRenderTarget(VRORenderTargetType type, int numAttachments, int numImages,
                                                             bool enableMipmaps, bool needsDepthStencil) = 0;
    virtual std::shared_ptr<VROVertexBuffer> newVertexBuffer(std::shared_ptr<VROData> data) = 0;
    virtual std::shared_ptr<VRORenderTarget> getDisplay() = 0;
    virtual std::shared_ptr<VROImagePostProcess> newImagePostProcess(std::shared_ptr<VROShaderProgram> shader) = 0;
    virtual std::shared_ptr<VROVideoTextureCache> newVideoTextureCache() = 0;
    virtual std::shared_ptr<VROSound> newSound(std::shared_ptr<VROSoundData> data, VROSoundType type) = 0;
    virtual std::shared_ptr<VROSound> newSound(std::string resource, VROResourceType resourceType, VROSoundType type) = 0;
    virtual std::shared_ptr<VROAudioPlayer> newAudioPlayer(std::shared_ptr<VROSoundData> data) = 0;
    virtual std::shared_ptr<VROAudioPlayer> newAudioPlayer(std::string path, bool isLocal) = 0;
    virtual std::shared_ptr<VROTypefaceCollection> newTypefaceCollection(std::string typefaces, int size, VROFontStyle style, VROFontWeight weight) = 0;
    virtual void setSoundRoom(float sizeX, float sizeY, float sizeZ, std::string wallMaterial,
                              std::string ceilingMaterial, std::string floorMaterial) = 0;
    virtual void setBlendingMode(VROBlendMode mode) = 0;
    
    virtual std::shared_ptr<VROFrameScheduler> getFrameScheduler() = 0;
    virtual void *getGraphicsContext() = 0;
};

#endif /* VRODriver_hpp */
