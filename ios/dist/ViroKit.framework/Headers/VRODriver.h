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
    
    virtual VROGeometrySubstrate *newGeometrySubstrate(const VROGeometry &geometry) = 0;
    virtual VROMaterialSubstrate *newMaterialSubstrate(VROMaterial &material) = 0;
    virtual VROTextureSubstrate *newTextureSubstrate(VROTextureType type,
                                                     VROTextureFormat format,
                                                     VROTextureInternalFormat internalFormat,
                                                     VROMipmapMode mipmapMode,
                                                     std::vector<std::shared_ptr<VROData>> &data,
                                                     int width, int height, std::vector<uint32_t> mipSizes) = 0;
    virtual VROVideoTextureCache *newVideoTextureCache() = 0;
    virtual std::shared_ptr<VROSound> newSound(std::shared_ptr<VROSoundData> data, VROSoundType type) = 0;
    virtual std::shared_ptr<VROSound> newSound(std::string path, VROSoundType type, bool local) = 0;
    virtual std::shared_ptr<VROAudioPlayer> newAudioPlayer(std::shared_ptr<VROSoundData> data) = 0;
    virtual std::shared_ptr<VROAudioPlayer> newAudioPlayer(std::string path, bool isLocal) = 0;
    virtual std::shared_ptr<VROTypeface> newTypeface(std::string typeface, int size) = 0;
    virtual void setSoundRoom(float sizeX, float sizeY, float sizeZ, std::string wallMaterial,
                              std::string ceilingMaterial, std::string floorMaterial) = 0;

};

#endif /* VRODriver_hpp */
