//
//  VROSoundDataGVR.h
//  ViroRenderer
//
//  Created by Andy Chu on 1/30/17.
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

#ifndef VROSoundDataGVR_h
#define VROSoundDataGVR_h

#include <functional>
#include "VROSoundData.h"
#include "VROModelIOUtil.h"

enum class VROSoundDataStatus {
    NotLoaded,
    Ready,
    Error
};

class VROSoundDataGVR : public VROSoundData, public std::enable_shared_from_this<VROSoundDataGVR> {
public:

    static std::shared_ptr<VROSoundDataGVR> create(std::string resource, VROResourceType type);
    VROSoundDataGVR(std::string resource, VROResourceType type);
    virtual ~VROSoundDataGVR();

    std::string getLocalFilePath();
    void setDelegate(std::weak_ptr<VROSoundDataDelegate> delegate);

    void ready(std::string fileName, bool isTemp);
    void error(std::string err);

private:
    std::string _resource;
    VROResourceType _resourceType;
    std::string _localPath;
    VROSoundDataStatus _status;
    std::string _error;
    bool _deleteFileOnDestroy;
    
    void setup(std::string resource, VROResourceType resourceType);
    void notifyDelegateOfStatus();
    void loadSoundFromURL(std::string path,
                          std::function<void(std::string)> onFinish,
                          std::function<void(std::string)> onError);
    void loadSoundFromResource(std::string path,
                               std::function<void(std::string)> onFinish);
};
#endif /* VROSoundDataGVR_h */
