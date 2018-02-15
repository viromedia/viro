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
#import "VROSoundData.h"

enum class VROSoundDataStatus {
    NotLoaded,
    Ready,
    Error
};

class VROSoundDataGVR : public VROSoundData, public std::enable_shared_from_this<VROSoundDataGVR> {

public:

    // If creating a non-local file, you *have* to use the static create function
    static std::shared_ptr<VROSoundDataGVR> create(std::string path, bool local);
    VROSoundDataGVR(std::string path, bool local);
    virtual ~VROSoundDataGVR();

    std::string getLocalFilePath();
    
    void setDelegate(std::weak_ptr<VROSoundDataDelegate> delegate);

    void ready(std::string fileName);
    void error(std::string err);

private:
    std::string _path;
    std::string _localPath;
    bool _local = false;
    VROSoundDataStatus _status;
    std::string _error;

    void setup();
    void notifyDelegateOfStatus();
    void loadSoundFromURL(std::string path,
                          std::function<void(std::string)> onFinish,
                          std::function<void(std::string)> onError);
    void loadSoundFromResource(std::string path,
                               std::function<void(std::string)> onFinish);
};
#endif /* VROSoundDataGVR_h */
