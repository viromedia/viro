//
//  VROSoundData.h
//  ViroRenderer
//
//  Created by Andy Chu on 1/30/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROSoundData_h
#define VROSoundData_h

#include <string>
#include <memory>
#include "VROSoundDataDelegate.h"

class VROSoundData {

public:

    virtual ~VROSoundData() {}

    virtual std::string getLocalFilePath() = 0;

    virtual void setDelegate(std::weak_ptr<VROSoundDataDelegate> delegate) {
        _delegate = delegate;
    }

protected:
    std::weak_ptr<VROSoundDataDelegate> _delegate;
};

#endif /* VROSoundData_h */
