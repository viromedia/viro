//
//  VROVideoTextureCache.hpp
//  ViroRenderer
//
//  Created by Raj Advani on 5/19/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROVideoTextureCache_hpp
#define VROVideoTextureCache_hpp

#include <stdio.h>
#include <memory>
#import <AVFoundation/AVFoundation.h>
#import <CoreVideo/CVMetalTextureCache.h>

class VROTextureSubstrate;

class VROVideoTextureCache {
    
public:
    
    virtual ~VROVideoTextureCache() {}
    
    /*
     For converting generic RGB buffers to textures.
     */
    virtual std::unique_ptr<VROTextureSubstrate> createTextureSubstrate(CMSampleBufferRef sampleBuffer) = 0;
    virtual std::unique_ptr<VROTextureSubstrate> createTextureSubstrate(CVPixelBufferRef pixelBuffer) = 0;
    
    /*
     Convert a YCbCr pixel buffer into two textures; the first will be the Y texture
     and the second the CbCr texture.
     */
    virtual std::vector<std::unique_ptr<VROTextureSubstrate>> createYCbCrTextureSubstrates(CVPixelBufferRef pixelBuffer) = 0;
    
private:
    
};

#endif /* VROVideoTextureCache_hpp */
