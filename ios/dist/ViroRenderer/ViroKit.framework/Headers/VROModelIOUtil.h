//
//  VROModelIOUtil.h
//  ViroRenderer
//
//  Created by Raj Advani on 5/2/17.
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

#ifndef VROModelIOUtil_h
#define VROModelIOUtil_h

#include <string>
#include <map>
#include <memory>

class VROTexture;
class VRONode;
class VRODriver;

/*
 The type of file within which the model is stored. On Android, use URL with file:///android-asset/
 to load assets.
 */
enum class VROResourceType {
    LocalFile,        // Local file path
    URL,              // URL (remote or local)
    BundledResource,  // App-bundled resource
};

/*
 Static utilities for Model IO.
 */
class VROModelIOUtil {
    
public:

    /*
     Load the texture with the given name, from the given base path (or base URL). First check
     if the texture exists in the provided resourceMap or cache. If the texture could not be
     loaded, returns an empty shared_ptr.
     
     Set sRGB to true to gamma-uncorrect the texture into linear RGB when sampling. This should
     only be used for color (diffuse) textures, and not for textures that are *already* linear
     (e.g. specular, normal, etc.).
     */
    static void loadTextureAsync(const std::string &name, const std::string &base, VROResourceType type, bool sRGB,
                                 std::shared_ptr<std::map<std::string, std::string>> resourceMap,
                                 std::shared_ptr<std::map<std::string, std::shared_ptr<VROTexture>>> textureCache,
                                 std::function<void(std::shared_ptr<VROTexture> texture)> onFinished);

    /*
     Retrieve the given resource, returning the path to it on the local filesystem.
     The isTemp flag is set to true if the flag is temporary and should be deleted
     after use. The success flag is set to false on failure.
     */
    static std::string retrieveResource(std::string resource, VROResourceType type,
                                        bool *isTemp, bool *success);
    
    /*
     Retrieve the given resource asynchronously from the rendering thread. Invoke the given callback
     on success or failure. The callback contains the local filepath of the retrieved resource.
     The callback will be invoked on the rendering thread.
     */
    static void retrieveResourceAsync(std::string resource, VROResourceType type,
                                      std::function<void(std::string, bool)> onSuccess,
                                      std::function<void()> onFailure);
    
    /*
     Copy the resource or map of resources into a location where they can be loaded by the model
     loader, and return the new paths.
     
     This performs no action on local files.
     */
    static std::shared_ptr<std::map<std::string, std::string>> createResourceMap(const std::map<std::string, std::string> &resourceMap, VROResourceType type);

    /*
     Recursively hydrate all geometries and textures in that descend from the given node.
     */
    static void hydrateNodes(std::shared_ptr<VRONode> node, std::shared_ptr<VRODriver> &driver);
    
    /*
     Upload all resources used by the given node (and its children) asynchronously on the rendering
     thread, as time permits each frame. When finished, the provided callback will be invoked.
     */
    static void hydrateAsync(std::shared_ptr<VRONode> node, std::function<void()> callback,
                             std::shared_ptr<VRODriver> &driver);
    
private:
    
    /*
     Recursive helper function for async hydration.
     */
    static void hydrateAsync(std::shared_ptr<VRONode> node, std::function<void()> callback, int *unhydratedTextureCount,
                             std::shared_ptr<VRODriver> &driver);
    
    /*
     Helper function for loadTextureAsync. Loads the texture at the given local file path.
     */
    static std::shared_ptr<VROTexture> loadLocalTexture(std::string name, std::string path,
                                                        bool sRGB, bool isTemp);
    
};

#endif /* VROModelIOUtil_h */
