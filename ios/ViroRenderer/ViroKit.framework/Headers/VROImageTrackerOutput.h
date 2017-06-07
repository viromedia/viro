//
//  VROImageTrackerOutput.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROImageTrackerOutput_hpp
#define VROImageTrackerOutput_hpp

#include <memory>
#include "opencv2/core/core.hpp"

class VROImageTrackerOutput {
public:
    bool found;
    std::vector<cv::Point2f> corners;
    cv::Mat translation;
    cv::Mat rotation;

    static std::shared_ptr<VROImageTrackerOutput> createFalseOutput();
    VROImageTrackerOutput(bool found);
    VROImageTrackerOutput(bool found, std::vector<cv::Point2f> corners, cv::Mat translation, cv::Mat rotation);
    ~VROImageTrackerOutput();
    
private:
    
};

#endif /* VROImageTrackerOutput_hpp */
