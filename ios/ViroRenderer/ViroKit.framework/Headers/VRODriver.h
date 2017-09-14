//
//  VRODriver.h
//  ViroRenderer
//
//  Created by Raj Advani on 4/21/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

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
class VROData;
class VROImage;
class VROVideoTextureCache;
class VROSound;
class VROAudioPlayer;
class VROTypeface;
class VROFrameTimer;
class VRORenderTarget;
class VRORenderContext;
class VROShaderProgram;
class VROImagePostProcess;

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

/*
 The driver is used to interface with the rendering subsystem (OpenGL,
 Metal, etc.).
 */
class VRODriver {
    
public:
    
    virtual ~VRODriver() {}
    
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
    virtual void setDepthWritingEnabled(bool enabled) = 0;
    virtual void setDepthReadingEnabled(bool enabled) = 0;
    virtual void setStencilTestEnabled(bool enabled) = 0;
    virtual void setCullMode(VROCullMode cullMode) = 0;
    virtual void setColorWritingEnabled(bool enabled) = 0;
    virtual void bindShader(std::shared_ptr<VROShaderProgram> program) = 0;
    virtual void unbindShader() = 0;
    
    /*
     If gamma correction is not enabled, then gamma correction framebuffer writes
     will be disabled, and sRGB texture sampling will be disabled (meaning we will do all
     shader compuatations in non-linear gamma 2.2 space, which is bad).
     */
    virtual bool isGammaCorrectionEnabled() = 0;
    
    /*
     Return true if bloom rendering is enabled. If so, materials that exceed their
     bloom threshold will glow.
     */
    virtual bool isBloomEnabled() = 0;
    
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
    virtual std::shared_ptr<VRORenderTarget> newRenderTarget(VRORenderTargetType type, int numAttachments, int numImages) = 0;
    virtual std::shared_ptr<VRORenderTarget> getDisplay() = 0;
    virtual std::shared_ptr<VROImagePostProcess> newImagePostProcess(std::shared_ptr<VROShaderProgram> shader) = 0;
    virtual std::shared_ptr<VROVideoTextureCache> newVideoTextureCache() = 0;
    virtual std::shared_ptr<VROSound> newSound(std::shared_ptr<VROSoundData> data, VROSoundType type) = 0;
    virtual std::shared_ptr<VROSound> newSound(std::string path, VROSoundType type, bool local) = 0;
    virtual std::shared_ptr<VROAudioPlayer> newAudioPlayer(std::shared_ptr<VROSoundData> data) = 0;
    virtual std::shared_ptr<VROAudioPlayer> newAudioPlayer(std::string path, bool isLocal) = 0;
    virtual std::shared_ptr<VROTypeface> newTypeface(std::string typeface, int size) = 0;
    virtual void setSoundRoom(float sizeX, float sizeY, float sizeZ, std::string wallMaterial,
                              std::string ceilingMaterial, std::string floorMaterial) = 0;
    virtual void setBlendingMode(VROBlendMode mode) = 0;
};

#endif /* VRODriver_hpp */
