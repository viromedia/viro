//
//  VROARImageDatabase.cpp
//
//  Created by Andy Chu on 11/3/17.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROARImageDatabase_h
#define VROARImageDatabase_h

#include <cstdint>
#include "VROPlatformUtil.h"

class VROARImageDatabase {
public:
    VROARImageDatabase(uint8_t *fileData, int64_t length) :
        _fileData(fileData),
        _length(length) {}

    ~VROARImageDatabase() {}

    uint8_t *getFileData() {
        return _fileData;
    }

    int64_t getLength() {
        return _length;
    }

private:
    uint8_t *_fileData;
    int64_t _length;
};

#endif //VROARImageDatabase_h
