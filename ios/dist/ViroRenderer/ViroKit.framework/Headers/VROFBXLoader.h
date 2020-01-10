//
//  VROFBXLoader.h
//  ViroRenderer
//
//  Created by Raj Advani on 5/1/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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
#include "VROModelIOUtil.h"

class VRONode;
class VROTexture;
class VROGeometry;
class VROSkinner;
class VROSkeleton;
class VROTaskQueue;
class VROSkeletalAnimation;
class VROKeyframeAnimation;

namespace viro {
    class Node;
    class Node_Geometry;
    class Node_Skeleton;
    class Node_Geometry_Skin;
    class Node_SkeletalAnimation;
    class Node_KeyframeAnimation;
}

class VROFBXLoader {

public:
    
    /*
     Load the FBX node subgraph at the given resource, into the given node.
     For all dependent resources (e.g. textures) found, locate them in the
     parent folder of the resource.
     
     The FBX is loaded in the background. Afterward, the geometry and child-nodes
     are injected into the node on the main (rendering) thread, and the given
     callback is invoked.
     */
    static void loadFBXFromResource(std::string file, VROResourceType type,
                                    std::shared_ptr<VRONode> destination,
                                    std::shared_ptr<VRODriver> driver,
                                    std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish = nullptr);
    static void loadFBXFromResources(std::string file, VROResourceType type,
                                     std::shared_ptr<VRONode> destination,
                                     std::map<std::string, std::string> resourceMap,
                                     std::shared_ptr<VRODriver> driver,
                                     std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish = nullptr);

private:
    
    static void injectFBX(std::shared_ptr<VRONode> fbxNode,
                          std::shared_ptr<VRONode> node,
                          std::shared_ptr<VRODriver> driver,
                          std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish);
    
    static void readFBXProtobufAsync(std::string resource, VROResourceType type, std::shared_ptr<VRONode> node,
                                     std::string path, bool isTemp, bool loadingTexturesFromResourceMap,
                                     std::map<std::string, std::string> resourceMap,
                                     std::shared_ptr<VRODriver> driver,
                                     std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish);
    
    /*
     Load the FBX subgraph for the given file. The top-level node returned here is a dummy; all the
     data is stored in its children.
     */
    static std::shared_ptr<VRONode> loadFBX(viro::Node &node_pb, std::shared_ptr<VRONode> node, std::string base, VROResourceType type,
                                            std::shared_ptr<std::map<std::string, std::string>> resourceMap,
                                            std::shared_ptr<std::map<std::string, std::shared_ptr<VROTexture>>> textureCache,
                                            std::shared_ptr<VROTaskQueue> taskQueue,
                                            std::shared_ptr<VRODriver> driver);
    
    static std::shared_ptr<VRONode> loadFBXNode(const viro::Node &node_pb,
                                                std::shared_ptr<VROSkeleton> skeleton,
                                                std::string base, VROResourceType type,
                                                std::shared_ptr<std::map<std::string, std::string>> resourceMap,
                                                std::shared_ptr<std::map<std::string, std::shared_ptr<VROTexture>>> textureCache,
                                                std::shared_ptr<VROTaskQueue> taskQueue,
                                                std::shared_ptr<VRODriver> driver);
    
    static std::shared_ptr<VROGeometry> loadFBXGeometry(const viro::Node_Geometry &geo_pb,
                                                        std::string base, VROResourceType type,
                                                        std::shared_ptr<std::map<std::string, std::string>> resourceMap,
                                                        std::shared_ptr<std::map<std::string, std::shared_ptr<VROTexture>>> textureCache,
                                                        std::shared_ptr<VROTaskQueue> taskQueue,
                                                        std::shared_ptr<VRODriver> driver);
    
    static std::shared_ptr<VROSkeleton> loadFBXSkeleton(const viro::Node_Skeleton &skeleton_pb);
    static std::shared_ptr<VROSkinner> loadFBXSkinner(const viro::Node_Geometry_Skin &skin_pb,
                                                      std::shared_ptr<VROSkeleton> skeleton,
                                                      std::shared_ptr<VRODriver> driver);
    static std::shared_ptr<VROSkeletalAnimation> loadFBXSkeletalAnimation(const viro::Node_SkeletalAnimation &animation_pb,
                                                                          std::shared_ptr<VROSkinner> skinner);
    static std::shared_ptr<VROKeyframeAnimation> loadFBXKeyframeAnimation(const viro::Node_KeyframeAnimation &animation_pb);
    
    /*
     Remove nodes that do not have any geometry in their sub-graph. These nodes
     are empty cruft that do not need to be loaded into the renderer (in fact if
     they are loaded in, they can cause significant CPU churn as we recurse down
     them each frame).
     */
    static void trimEmptyNodes(std::shared_ptr<VRONode> node);
    static bool nodeHasGeometryRecursive(std::shared_ptr<VRONode> node);
    
};

#endif /* VROFBXLoader_h */
