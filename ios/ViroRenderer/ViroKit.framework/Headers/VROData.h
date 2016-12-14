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
 Holds onto an arbitrary block of bytes.
 */
class VROData {
    
public:
    
    /*
     Construct a new VROData. If copy is true, the bytes will be 
     copied into this object; otherwise, this object will assume
     ownership of the bytes.
     */
    VROData(void *data, int dataLength, bool copy = true);
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
    
};

#endif /* VROData_h */
