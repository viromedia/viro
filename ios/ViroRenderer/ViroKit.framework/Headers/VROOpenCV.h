//
//  VROOpenCV.h
//  ViroRenderer
//
//  Created by Andy Chu on 5/25/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROOpenCV_h
#define VROOpenCV_h

#include <stdio.h>
#include <string>
#include <ViroKit/ViroKit.h>
#include "opencv2/core/core.hpp"


class VROOpenCV {
  
public:
    VROOpenCV();
    virtual ~VROOpenCV() = 0;

    static void runEdgeDetection(const char* inputFile, const char* outputFile);
  
    static void runEdgeDetection(cv::Mat inputFile, cv::Mat outputFile);
};
#endif /* VROOpenCV_h */
