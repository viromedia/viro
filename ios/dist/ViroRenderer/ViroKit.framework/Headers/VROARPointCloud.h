//
//  VROARPointCloud.h
//  ViroKit
//
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

#ifndef VROARPointCloud_h
#define VROARPointCloud_h

#include <cstdint>
#include <vector>

class VROVector4f;
class VROMatrix3f;

// TODO: possible need to guard uint64_t usage with #ifdef UINT64_MAX, seems fine on iOS/Android
class VROARPointCloud {
public:
    VROARPointCloud() {}
    VROARPointCloud(std::vector<VROVector4f> points, std::vector<uint64_t> identifiers) :
    _points(points),
    _identifiers(identifiers) {}
    
    ~VROARPointCloud() {}
    
    /*
     Retrieves the point that make up this point cloud. Note: the 4th value in the
     vector is a "confidence" value only available on Android.
     */
    std::vector<VROVector4f> getPoints() {
        return _points;
    }
    
    /*
     Retrieves the identifiers corresponding to each point. Note: iOS only (it's empty
     on Android).
     */
    std::vector<uint64_t> getIdentifiers() {
        return _identifiers;
    }
    
private:
    std::vector<VROVector4f> _points;
    std::vector<uint64_t> _identifiers; // used only on iOS (Android does not provide identifiers).
};

#endif /* VROARPointCloud_h */
