//
//  VROTextureUtil.h
//  ViroRenderer
//
//  Created by Raj Advani on 3/21/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROTextureUtil_h
#define VROTextureUtil_h

#include <memory>

class VROTexture;
class VRODriver;
class VROData;
enum class VROTextureFormat;
enum class VROTextureType;

class VROTextureUtil {
    
public:
    
    /*
     Load a texture with an ASTC header. Read the width and height from the texture then
     strip the header out and return the raw texture data.
     */
    static std::shared_ptr<VROTexture> loadASTCTexture(const uint8_t *data, int length, VROTextureType type,
                                                       VRODriver *driver = nullptr);
    
    /*
     Read a texture file with an ASTC header. Read the width and height from the header then
     strip it out and return the raw texture data.
     */
    static std::shared_ptr<VROData> readASTCHeader(const uint8_t *data, int length, VROTextureFormat *outFormat,
                                                   int *outWidth, int *outHeight);
    
};

#endif /* VROTextureUtil_h */
