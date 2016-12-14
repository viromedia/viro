//
//  VROLoader.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/5/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROLoader_h
#define VROLoader_h

#include <stdio.h>
#include <vector>
#include <string>
#include <memory>
#include <ModelIO/ModelIO.h>
#include "VROGeometrySource.h"
#include "VROGeometryElement.h"
#include "VRORenderContext.h"

class VRONode;
class VROTexture;
class VROVector4f;

class VROLoader {
    
public:
    
    static std::vector<std::shared_ptr<VRONode>> loadURL(NSURL *url);
    
private:
    
    static std::shared_ptr<VRONode> loadMesh(MDLMesh *mesh);
    
    static VROGeometrySourceSemantic parseSemantic(NSString *string);
    static std::pair<bool, int> getBytesPerComponent(MDLVertexFormat format);
    static int getComponentsPerVertex(MDLVertexFormat format);
    static int parseIndexSize(MDLIndexBitDepth depth);
    static VROGeometryPrimitiveType parsePrimitive(MDLGeometryType type);
    
    static VROVector4f parseColor(CGColorRef colorRef);
    static std::shared_ptr<VROTexture> parseTexture(MDLTextureSampler *sampler);
    
    static int getPrimitiveCount(int indexCount, VROGeometryPrimitiveType primitiveType);
    
};


#endif /* VROLoader_h */
