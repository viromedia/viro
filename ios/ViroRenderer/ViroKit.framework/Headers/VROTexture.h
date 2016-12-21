//
//  VROTexture.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/17/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROTexture_h
#define VROTexture_h

#include <memory>
#include <vector>
#include "VRODefines.h"

class VROTextureSubstrate;
class VRODriver;
class VROImage;
class VROData;

enum class VROTextureType {
    None = 1,
    Texture2D = 2,
    TextureCube = 4,
    TextureEGLImage = 8
};

enum class VROTextureFormat {
    ETC2,
    ASTC_4x4_LDR,
    RGBA8,
};

class VROTexture {
    
public:
    
    /*
     Create a new VROTexture with no underlying image data.
     The image data must be injected via setImage*() or setSubstrate().
     */
    VROTexture(VROTextureType type);
    
    /*
     Create a new VROTexture with the given underlying substrate.
     */
    VROTexture(VROTextureType type, std::unique_ptr<VROTextureSubstrate> substrate);
    
    /*
     Create a new VROTexture from a VROImage. If a render context is supplied, then
     the texture will be prewarmed.
     */
    VROTexture(std::shared_ptr<VROImage> image, VRODriver *driver = nullptr);
    VROTexture(std::vector<std::shared_ptr<VROImage>> &images, VRODriver *driver = nullptr);
    
    /*
     Create a new VROTexture from the given raw data in the given format.
     */
    VROTexture(VROTextureType type, VROTextureFormat format,
               std::shared_ptr<VROData> data, int width, int height,
               VRODriver *driver = nullptr);
    
    virtual ~VROTexture();
    
    VROTextureType getType() const {
        return _type;
    }
    uint32_t getTextureId() const {
        return _textureId;
    }
    
    /*
     Get the texture ready for usage now, in advance of when it's visible. If not invoked,
     the texture will be initialized when it is made visible.
     */
    void prewarm(VRODriver &driver);
    
    VROTextureSubstrate *const getSubstrate(VRODriver &driver);
    void setSubstrate(std::unique_ptr<VROTextureSubstrate> substrate);
    
private:
    
    uint32_t _textureId;
    const VROTextureType _type;
    
    /*
     The image is retained until the texture is hydrated, after which the
     substrate is populated.
     
     Vector of images is used for cube textures.
     */
    std::shared_ptr<VROImage> _image;
    std::vector<std::shared_ptr<VROImage>> _imagesCube;
    
    /*
     If the underlying texture is compressed, its raw data is retined until the
     substrate is populated.
     */
    std::shared_ptr<VROData> _data;
    VROTextureFormat _format;
    int _width, _height;
    
    /*
     Representation of the texture in the underlying hardware.
     */
    std::unique_ptr<VROTextureSubstrate> _substrate;
    
    /*
     Converts the image(s) into a substrate.
     */
    void hydrate(VRODriver &driver);
    
};

#endif /* VROTexture_h */
