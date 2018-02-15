//
//  VROARPointCloud.h
//  ViroKit
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

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
