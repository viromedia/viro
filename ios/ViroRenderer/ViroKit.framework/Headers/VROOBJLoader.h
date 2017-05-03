//
//  VROOBJLoader.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/13/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROOBJLoader_h
#define VROOBJLoader_h

#include <stdio.h>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <functional>
#include "VROGeometrySource.h"
#include "VROGeometryElement.h"
#include "tiny_obj_loader.h"

class VRONode;
class VROTexture;
class VROGeometry;

class VROOBJLoader {
    
public:
    
    /*
     Load the OBJ file at the given URL or file. For all dependent resources
     (e.g. textures) found, locate them by prepending the given baseURL or baseDir.
     
     If async is true, an empty node is immediately returned while the OBJ is
     loaded in the background. Afterward, the geometry is injected into the node
     on the main (rendering) thread, and the given callback is invoked.
     
     If async is false, the callback is still executed.
     */
    static std::shared_ptr<VRONode> loadOBJFromURL(std::string url, std::string baseURL,
                                                   bool async = false, std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish = nullptr);
    static std::shared_ptr<VRONode> loadOBJFromFile(std::string file, std::string baseDir,
                                                    bool async = false, std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish = nullptr);
    static std::shared_ptr<VRONode> loadOBJFromFileWithResources(std::string file, std::map<std::string, std::string> resourceMap,
                                                                 bool async = false, std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish = nullptr);
private:
    
    static void injectOBJ(std::shared_ptr<VROGeometry> geometry, std::shared_ptr<VRONode> node,
                          std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish);
    static std::shared_ptr<VROGeometry> loadOBJ(std::string file, std::string base, bool isBaseURL);
    static std::shared_ptr<VROGeometry> loadOBJ(std::string file,
                                                std::map<std::string, std::string> resourceMap);

    static std::shared_ptr<VROGeometry> processOBJ(tinyobj::attrib_t attrib,
                                                   std::vector<tinyobj::shape_t> &shapes,
                                                   std::vector<tinyobj::material_t> &materials,
                                                   std::string base,
                                                   bool isBaseURL,
                                                   std::map<std::string, std::string> *resourceMap = nullptr);
};

#endif /* VROOBJLoader_h */
