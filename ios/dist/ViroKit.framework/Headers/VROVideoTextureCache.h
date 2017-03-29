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
    
    virtual std::unique_ptr<VROTextureSubstrate> createTextureSubstrate(CMSampleBufferRef sampleBuffer) = 0;
    virtual std::unique_ptr<VROTextureSubstrate> createTextureSubstrate(CVPixelBufferRef pixelBuffer) = 0;
    
private:
    
};

#endif /* VROVideoTextureCache_hpp */
