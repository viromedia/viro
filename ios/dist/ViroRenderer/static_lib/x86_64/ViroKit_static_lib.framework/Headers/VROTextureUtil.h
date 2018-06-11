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
#include <vector>
#include "VROStringUtil.h"
#include "VROTexture.h"

class VRODriver;
class VROData;
enum class VROTextureFormat;
enum class VROTextureType;

class VROTextureUtil {
    
public:
    
    /*
     Read a texture file with an ASTC header. Read the width and height from the header then
     strip it out and return the raw texture data.
     */
    static std::shared_ptr<VROData> readASTCHeader(const uint8_t *data, int length, VROTextureFormat *outFormat,
                                                   int *outWidth, int *outHeight);
    
    /*
     Read a texture file with a PKM header. Read the width and height from the header then
     strip it out and return the raw texture data, with successive mipmap levels concatenated
     contiguously together.
     
     The size of each mipmap level is returned in the outMipmaps vector.
     */
    static std::shared_ptr<VROData> readKTXHeader(const uint8_t *data, uint32_t length, VROTextureFormat *outFormat,
                                                  int *outWidth, int *outHeight, std::vector<uint32_t> *outMipSizes);
    
    /*
     Read a texture file with a VHD header. Read the width and height from the header then
     strip it out and return the raw texture data, with successive mipmap levels concatenated
     contiguously together.
          
     The size of each mipmap level is returned in the outMipmaps vector.
     */
    static std::shared_ptr<VROData> readVHDHeader(const std::string &data, VROTextureFormat *outFormat,
                                                  int *outWidth, int *outHeight, std::vector<uint32_t> *outMipSizes);

    /**
     Returns the stereo mode that corresponds to the given string.
     If none is found VROStereoMode::None will be returned.
     */
    static VROStereoMode getStereoModeForString(std::string tag);
    
};

#endif /* VROTextureUtil_h */
