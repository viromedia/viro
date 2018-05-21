//
//  VROSoundDataGVR.h
//  ViroRenderer
//
//  Created by Andy Chu on 1/30/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

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
