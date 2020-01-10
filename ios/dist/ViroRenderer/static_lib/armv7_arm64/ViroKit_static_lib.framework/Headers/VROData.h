//
//  VROData.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/17/15.
//  Copyright © 2015 Viro Media. All rights reserved.
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
