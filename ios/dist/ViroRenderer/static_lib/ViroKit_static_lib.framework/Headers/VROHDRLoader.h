//
//  VROHDRLoader.h
//  ViroKit
//
//  Created by Raj Advani on 1/22/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROHDRLoader_h
#define VROHDRLoader_h

#include <stdio.h>
#include <string>
#include <memory>

class VROTexture;
enum class VROTextureInternalFormat;

/*
 Converts HDR images to RGB9_E5 format so that they can be read by OpenGL ES.
 */
class VROHDRLoader {
public:
    
    /*
     Loads the Radiance HDR texture (.hdr) at the given path. The data will
     be internally stored in RGB9_E5 format.
     */
    static std::shared_ptr<VROTexture> loadRadianceHDRTexture(std::string hdrPath);
    
private:
    
    static std::shared_ptr<VROTexture> loadTexture(float *data, int width, int height,
                                                   int componentsPerPixel);
    
};

#endif /* VROHDRLoader_h */
