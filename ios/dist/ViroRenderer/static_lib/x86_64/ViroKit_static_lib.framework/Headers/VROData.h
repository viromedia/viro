//
//  VROData.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/17/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROData_h
#define VROData_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 Defines how the VROData holds onto its underlying data.
 
 Copy: data is copied on construction, freed on destruction
 Move: data is assigned on construction, freed on destruction
 Wrap: data is assigned on construction
 */
enum class VRODataOwnership {
    Copy,
    Move,
    Wrap
};

/*
 Holds onto an arbitrary block of bytes.
 */
class VROData {
    
public:
    
    /*
     Construct a new VROData. Default ownership semanatics are
     Copy.
     */
    VROData(void *data, int dataLength, VRODataOwnership ownership = VRODataOwnership::Copy);

    /*
     Construct a new VROData, copying a dataLength's worth of bytes, starting
     at a byteOffset into the given data (allows for const data input).
     */
    VROData(const void *data, int dataLength, int byteOffset = 0);

    ~VROData();
    
    void *const getData() {
        return _data;
    }
    int getDataLength() const {
        return _dataLength;
    }
    
private:
    
    void *_data;
    int _dataLength;
    
    VRODataOwnership _ownership;
    
};

#endif /* VROData_h */
