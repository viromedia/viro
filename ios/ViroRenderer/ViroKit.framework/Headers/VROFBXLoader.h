//
//  VROFBXLoader.h
//  ViroRenderer
//
//  Created by Raj Advani on 5/1/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROFBXLoader_h
#define VROFBXLoader_h

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <functional>
#include "VROGeometrySource.h"
#include "VROGeometryElement.h"
#include "VROMaterial.h"

class VRONode;
class VROTexture;
class VROGeometry;

namespace viro {
    class Node;
    class Node_Geometry;
}

class VROFBXLoader {
    
public:
    
    /*
     Load the FBX node subgraph at the given URL or file. For all dependent resources
     (e.g. textures) found, locate them by prepending the given baseURL or baseDir.
     
     If async is true, an empty node is immediately returned while the FBX is
     loaded in the background. Afterward, the geometry and child-nodes are injected 
     into the node on the main (rendering) thread, and the given callback is invoked.
     
     If async is false, the callback is still executed.
     */
    static std::shared_ptr<VRONode> loadFBXFromURL(std::string url, std::string baseURL,
                                                   bool async = false, std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish = nullptr);
    static std::shared_ptr<VRONode> loadFBXFromFile(std::string file, std::string baseDir,
                                                    bool async = false, std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish = nullptr);
    static std::shared_ptr<VRONode> loadFBXFromFileWithResources(std::string file, std::map<std::string, std::string> resourceMap,
                                                                 bool async = false, std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish = nullptr);
private:
    
    static void injectFBX(std::shared_ptr<VRONode> fbxNode, std::shared_ptr<VRONode> node,
                          std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish);
    
    /*
     Load the FBX subgraph for the given file. The top-level node returned here is a dummy; all the
     data is stored in its children.
     */
    static std::shared_ptr<VRONode> loadFBX(std::string file, std::string base, bool isBaseURL,
                                            const std::map<std::string, std::string> *resourceMap);
    
    static std::shared_ptr<VRONode> loadFBXNode(const viro::Node &node_pb,
                                                std::string base, bool isBaseURL,
                                                const std::map<std::string, std::string> *resourceMap,
                                                std::map<std::string, std::shared_ptr<VROTexture>> &textureCache);
    
    static std::shared_ptr<VROGeometry> loadFBXGeometry(const viro::Node_Geometry &geo_pb,
                                                        std::string base, bool isBaseURL,
                                                        const std::map<std::string, std::string> *resourceMap,
                                                        std::map<std::string, std::shared_ptr<VROTexture>> &textureCache);
    
};

#endif /* VROFBXLoader_h */
