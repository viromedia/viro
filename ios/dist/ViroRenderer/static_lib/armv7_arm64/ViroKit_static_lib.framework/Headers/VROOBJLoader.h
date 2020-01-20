//
//  VROOBJLoader.h
//  ViroRenderer
//
//  Created by Raj Advani on 12/13/16.
//  Copyright © 2016 Viro Media. All rights reserved.
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
class VROTaskQueue;
enum class VROResourceType;

class VROOBJLoader {
    
public:
    
    /*
     Load the OBJ file at the given resource.  For all dependent resources
     (e.g. textures) found, locate them in the parent folder of the resource.
     
     The OBJ is loaded in the background. Afterward, the geometry is injected
     into the node on the main (rendering) thread, and the given callback is invoked.
     */
    static void loadOBJFromResource(std::string resource, VROResourceType type,
                                    std::shared_ptr<VRONode> destination,
                                    std::shared_ptr<VRODriver> driver,
                                    std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish = nullptr);
    static void loadOBJFromResources(std::string resource, VROResourceType type,
                                     std::shared_ptr<VRONode> destination,
                                     std::map<std::string, std::string> resourceMap,
                                     std::shared_ptr<VRODriver> driver,
                                     std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish = nullptr);

private:
    
    static void injectOBJ(std::shared_ptr<VROGeometry> geometry,
                          std::shared_ptr<VRONode> node,
                          std::shared_ptr<VRODriver> driver,
                          std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish);
    static void readOBJFileAsync(std::string resource, VROResourceType type, std::shared_ptr<VRONode> node,
                                 std::string path, bool isTemp, bool loadingTexturesFromResourceMap,
                                 std::map<std::string, std::string> resourceMap,
                                 std::shared_ptr<VRODriver> driver,
                                 std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish);

    static std::shared_ptr<VROGeometry> processOBJ(tinyobj::attrib_t &attrib,
                                                   std::vector<tinyobj::shape_t> &shapes,
                                                   std::vector<tinyobj::material_t> &materials,
                                                   std::string base,
                                                   VROResourceType type,
                                                   std::shared_ptr<std::map<std::string, std::string>> resourceMap,
                                                   std::shared_ptr<std::map<std::string, std::shared_ptr<VROTexture>>> textureCache,
                                                   std::shared_ptr<VROTaskQueue> taskQueue);
};

#endif /* VROOBJLoader_h */
