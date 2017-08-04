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
class VRORenderContext;

enum class VROSoundType;
enum class VROTextureType;
enum class VROTextureFormat;
enum class VROTextureInternalFormat;
enum class VROWrapMode;
enum class VROFilterMode;
enum class VROMipmapMode;

/*
 The driver is used to interface with the rendering subsystem (OpenGL,
 Metal, etc.).
 */
class VRODriver {
    
public:
    
    virtual ~VRODriver() {}
    
    // Provides the driver an opportunity to update any sub-components
    // with per-frame state. The willRenderFrame method is invoked just
    // prior to rendering, and didRenderFrame is invoked afterward. The
    // VROFrameTimer can be used by the driver to timebox its tasks so
    // as not to overrun frame time.
    virtual void willRenderFrame(const VRORenderContext &context) = 0;
    virtual void didRenderFrame(const VROFrameTimer &timer, const VRORenderContext &context) = 0;
    
    // Clear the stencil buffer with the given clear value
    virtual void clearStencil(int bits) = 0;
    
    // Clear the depth buffer
    virtual void clearDepth() = 0;
    
    // Clear the color buffer
    virtual void clearColor() = 0;
    
    // Clear both depth and color at the same time
    virtual void clearDepthAndColor() = 0;
    
    // Enable the color buffer for writing
    virtual void enableColorBuffer() = 0;
    
    // Disable the color buffer from writing
    virtual void disableColorBuffer() = 0;
    
    // Enable portal stencil functions. When writing, we INCR the stencil
    // buffer. When removing, we DECR the buffer. Finally when reading, we
    // make the stencil buffer read-only.
    virtual void enablePortalStencilWriting() = 0;
    virtual void enablePortalStencilRemoval() = 0;
    virtual void enablePortalStencilReading() = 0;
    
    // Set the reference bits for the stencil test. If passIfLess is
    // false, we pass the stencil test if ref equals the value in the
    // stencil buffer. If passIsLess is true, we pass the stencil test
    // if ref <= value in stencil buffer.
    virtual void setStencilPassBits(int bits, bool passIfLess) = 0;
    
    virtual VROGeometrySubstrate *newGeometrySubstrate(const VROGeometry &geometry) = 0;
    virtual VROMaterialSubstrate *newMaterialSubstrate(VROMaterial &material) = 0;
    virtual VROTextureSubstrate *newTextureSubstrate(VROTextureType type,
                                                     VROTextureFormat format,
                                                     VROTextureInternalFormat internalFormat,
                                                     VROMipmapMode mipmapMode,
                                                     std::vector<std::shared_ptr<VROData>> &data,
                                                     int width, int height, std::vector<uint32_t> mipSizes,
                                                     VROWrapMode wrapS, VROWrapMode wrapT,
                                                     VROFilterMode minFilter, VROFilterMode magFilter, VROFilterMode mipFilter) = 0;
    virtual std::shared_ptr<VROVideoTextureCache> newVideoTextureCache() = 0;
    virtual std::shared_ptr<VROSound> newSound(std::shared_ptr<VROSoundData> data, VROSoundType type) = 0;
    virtual std::shared_ptr<VROSound> newSound(std::string path, VROSoundType type, bool local) = 0;
    virtual std::shared_ptr<VROAudioPlayer> newAudioPlayer(std::shared_ptr<VROSoundData> data) = 0;
    virtual std::shared_ptr<VROAudioPlayer> newAudioPlayer(std::string path, bool isLocal) = 0;
    virtual std::shared_ptr<VROTypeface> newTypeface(std::string typeface, int size) = 0;
    virtual void setSoundRoom(float sizeX, float sizeY, float sizeZ, std::string wallMaterial,
                              std::string ceilingMaterial, std::string floorMaterial) = 0;

};

#endif /* VRODriver_hpp */
