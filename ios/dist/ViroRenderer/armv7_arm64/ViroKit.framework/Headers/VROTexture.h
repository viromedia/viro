//
//  VROTexture.h
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

#ifndef VROTexture_h
#define VROTexture_h

#include <memory>
#include <vector>
#include <string>
#include <functional>
#include "VRODefines.h"

// Constants for ETC2 ripped from NDKr9 headers
#define GL_COMPRESSED_RGB8_ETC2                          0x9274
#define GL_COMPRESSED_RGBA8_ETC2_EAC                     0x9278

class VROTextureSubstrate;
class VRODriver;
class VROImage;
class VROData;
class VROFrameScheduler;

enum class VROTextureType {
    None = 1,
    Texture2D = 2,
    TextureCube = 4,
    TextureEGLImage = 8
};

// Texture formats for source data
enum class VROTextureFormat {
    ETC2_RGBA8_EAC,
    ASTC_4x4_LDR,
    RGBA8,
    RGB565,
    RGB8,
    RGB9_E5,
    RGB16F,
};

// Texture formats for storage on the GPU
// (e.g. we can load an RGBA8 texture and store it as RGBA4 to
//       preserve GPU memory)
// Note we do not include RGB8 here because RGB8 internal formats
// in sRGB cannot automatically generate mipmaps. As a result, all
// RGBA8 formats will be converted to RGB8 internal formats.
enum class VROTextureInternalFormat {
    RGBA8,
    RGBA4,
    RGB565,
    YCBCR,
    RGB9_E5,
    RGB16F,
    RG8,
};

enum class VROMipmapMode {
    None,          // Do not use mipmaps
    Pregenerated,  // Mipmaps are baked into the texture data
    Runtime,       // Build mipmaps at texture loading time
};

enum class VROStereoMode {
    None = 1,       // No stereo is applied, image is fully represented in the texture.
    LeftRight = 2,  // Side by side stereoscopic image, with the left image shown to the left eye.
    RightLeft = 3,  // Side by side stereoscopic image, with the right image shown to the left eye.
    TopBottom = 4,  // Over/Under stereoscopic image, with the top image shown to the left eye.
    BottomTop = 5   // Over/Under stereoscopic image, with the bottom image shown to the left eye.
};

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

class VROTexture : public std::enable_shared_from_this<VROTexture> {
    
public:
    
    /*
     Create a new VROTexture with no underlying image data.
     The image data must be injected via setSubstrate(). This is
     the preferred constructor for multi-substrate textures
     (e.g. YCbCr).
     */
    VROTexture(VROTextureType type,
               VROTextureInternalFormat internalFormat,
               VROStereoMode stereoMode = VROStereoMode::None);
    
    /*
     Create a new VROTexture with the given underlying substrate.
     */
    VROTexture(VROTextureType type,
               VROTextureInternalFormat internalFormat,
               std::unique_ptr<VROTextureSubstrate> substrate,
               VROStereoMode stereoMode = VROStereoMode::None);
    
    /*
     Create a new VROTexture from a VROImage.
     */
    VROTexture(bool sRGB,
               VROMipmapMode mipmapMode,
               std::shared_ptr<VROImage> image,
               VROStereoMode stereoMode = VROStereoMode::None);

    VROTexture(bool sRGB,
               std::vector<std::shared_ptr<VROImage>> &images,
               VROStereoMode stereoMode = VROStereoMode::None);
    
    /*
     Create a new VROTexture from the given raw data in the given format.
     The format parameter defines the input format of the data, and
     internal format specifies how to store the texture in GPU memory.
     Set sRGB to true if the texture is in sRGB color space (gamma 2.2), and
     should be gamma-uncorrected into linear RGB space when sampled, for better
     accuracy during shader computations. This should *only* be used if gamma
     correction is enabled on the framebuffer or shader (to convert
     from linear back to gamma 2.2 space when writing to the framebuffer).
     */
    VROTexture(VROTextureType type,
               VROTextureFormat format,
               VROTextureInternalFormat internalFormat, bool sRGB,
               VROMipmapMode mipmapMode,
               std::vector<std::shared_ptr<VROData>> &data,
               int width, int height,
               std::vector<uint32_t> mipSizes,
               VROStereoMode stereoMode = VROStereoMode::None);
    
    virtual ~VROTexture();
    
    /*
     Upload this texture to the GPU asynchronously (on the rendering thread). Invoke
     the given callback when hydration is complete. No-op if the texture is already
     hydrated.
     */
    void hydrateAsync(std::function<void()> callback,
                      std::shared_ptr<VRODriver> &driver);
    
    /*
     Delete any rendering resources. Invoked prior to destruction, on the
     rendering thread.
     */
    virtual void deleteGL() {}
    
    VROTextureType getType() const {
        return _type;
    }
    uint32_t getTextureId() const {
        return _textureId;
    }
    
    /*
     Set or get a user-provided name.
     */
    std::string getName() const { return _name; }
    void setName(std::string name) {
        _name = name;
    }

    /*
     Get the texture ready for usage now, in advance of when it's visible. If not invoked,
     the texture will be initialized when it is made visible.
     */
    void prewarm(std::shared_ptr<VRODriver> driver);
    
    /*
     Get the substrates for this texture, loading them if necessary. If a immediate is false,
     then the substrates will be loaded asynchronously via the scheduler; otherwise, the
     substrates will be loaded immediately in a blocking fashion.
     */
    int getNumSubstrates() const;
    VROTextureSubstrate *getSubstrate(int index, std::shared_ptr<VRODriver> &driver, bool immediate);
    
    /*
     Textures may have their substrates set externally if they are created and
     managed elsewhere.
     */
    void setSubstrate(int index, std::unique_ptr<VROTextureSubstrate> substrate);

    VROTextureInternalFormat getInternalFormat() const {
        return _internalFormat;
    }
    VROStereoMode getStereoMode() const {
        return _stereoMode;
    }
    
    /*
     True if this texture has been uploaded to the GPU.
     */
    bool isHydrated() const;
    
    /*
     True if this texture has an alpha channel.
     */
    bool hasAlpha() const;
    
    /*
     Access and set wrap properties.
     */
    VROWrapMode getWrapS() const {
        return _wrapS;
    }
    void setWrapS(VROWrapMode wrapMode);
    VROWrapMode getWrapT() const {
        return _wrapT;
    }
    void setWrapT(VROWrapMode wrapMode);
    
    /*
     Access and set filtering properties.
     */
    VROFilterMode getMinificationFilter() const {
        return _minificationFilter;
    }
    void setMinificationFilter(VROFilterMode filter) {
        _minificationFilter = filter;
    }
    VROFilterMode getMagnificationFilter() const {
        return _magnificationFilter;
    }
    void setMagnificationFilter(VROFilterMode filter) {
        _magnificationFilter = filter;
    }
    VROFilterMode getMipFilter() const {
        return _mipFilter;
    }
    void setMipFilter(VROFilterMode filter) {
        _mipFilter = filter;
    }

    /*
     Width and height (available for any 2D texture created through an image).
     */
    int getWidth() const {
        return _width;
    }
    int getHeight() const {
        return _height;
    }

protected:
    int _width, _height;

private:
    
    uint32_t _textureId;
    const VROTextureType _type;
    
    /*
     User provided name, non-unique.
     */
    std::string _name;
    
    /*
     The image is retained until the texture is hydrated, after which the
     substrate is populated.
     
     Vector of images is used for cube textures.
     */
    std::vector<std::shared_ptr<VROImage>> _images;
    
    /*
     If the underlying texture is compressed, its raw data is retined until the
     substrate is populated. Cube textures contain six faces, all other
     textures will only have one element in this vector.
     */
    std::vector<std::shared_ptr<VROData>> _data;
    
    /*
     The format of the source data (_data).
     */
    VROTextureFormat _format;
    
    /*
     The format in which we want to store the data on the GPU.
     (defaults to RGBA8, and is ignored if we're using a compressed
     source data format: compressed textures are always stored in their
     source format).
     */
    VROTextureInternalFormat _internalFormat;

    /*
     The mipmap generation mode for this texture. Determines if mipmaps
     are loaded from the source data (pregenerated), generated at runtime,
     or not used at all. The _mipSizes vector indicates the compessed size
     of each mip-level in the source data, if _mipmapMode = Pregenerated.
     */
    VROMipmapMode _mipmapMode;
    std::vector<uint32_t> _mipSizes;
    
    /*
     Representation of the texture in the underlying hardware. Textures typically
     have one substrate, but some textures (e.g. YCbCr) may have multiple planes,
     each represented by a separate substrate.
     */
    std::vector<std::unique_ptr<VROTextureSubstrate>> _substrates;
    
    /*
     True if the underlying substrate should gamma-uncorrect the texture data
     into linear space when sampled. Typically only diffuse texture are stored
     in sRGB formats and therefore need gamma-uncorrection.
     */
    bool _sRGB;

    /*
     Represents the stereo property of the image, if any.
     */
    VROStereoMode _stereoMode;
    
    /*
     Wrap and filtering properties.
     */
    VROWrapMode _wrapS, _wrapT;
    VROFilterMode _minificationFilter, _magnificationFilter, _mipFilter;
    
    /*
     Callbacks invoked when the texture is hydrated.
     */
    std::vector<std::function<void()>> _hydrationCallbacks;

    /*
     Converts the image(s) into a substrate. May be asynchronously executed.
     */
    void hydrate(std::shared_ptr<VRODriver> &driver);
    
    /*
     Create a task to hydrate the texture.
     */
    std::string getHydrationTaskKey() const;
    std::function<void()> createHydrationTask(std::shared_ptr<VRODriver> &driver);
    
    /*
     Set the number of substrates to be used by this texture.
     */
    void setNumSubstrates(int numSubstrates);
    
    /*
     Get the number of substrates used by the given texture format.
     */
    int getNumSubstratesForFormat(VROTextureInternalFormat format) const;
};

#endif /* VROTexture_h */
