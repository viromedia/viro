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

class VROSoundDataGVR : public VROSoundData, public std::enable_shared_from_this<VROSoundDataGVR> {

public:

    // If creating a non-local file, you *have* to use the static create function
    static std::shared_ptr<VROSoundDataGVR> create(std::string path, bool local);
    VROSoundDataGVR(std::string path, bool local);
    virtual ~VROSoundDataGVR();

    std::string getLocalFilePath();
    
    void setDelegate(std::weak_ptr<VROSoundDataDelegate> delegate);

    void ready(std::string fileName);

private:
    std::string _path;
    std::string _localPath;
    bool _ready = false;

    void setup();
    void notifyDelegateIfReady();
    void loadSound(std::string path, std::function<void(std::string)> onFinish);
};
#endif /* VROSoundDataGVR_h */
