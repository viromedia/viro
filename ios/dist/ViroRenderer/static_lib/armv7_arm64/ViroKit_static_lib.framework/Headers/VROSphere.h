//
//  VROSphere.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/20/16.
//  Copyright © 2016 Viro Media. All rights reserved.
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

#ifndef VROSphere_h
#define VROSphere_h

#include "VROGeometry.h"

class VROSphere : public VROGeometry {
    
public:
    
    static std::shared_ptr<VROSphere> createSphere(float radius, int widthSegments, int heightSegments,
                                                   bool facesOutward);
    VROSphere(float radius, int widthSegments = 20, int heightSegments = 20, bool facesOutward = true);
    virtual ~VROSphere() {}

    void setRadius(float radius);
    void setWidthSegments(int widthSegments);
    void setHeightSegments(int heightSegments);
    void setFacesOutward(bool facesOutward);
    
private:

    float _radius;
    int _widthSegments, _heightSegments;
    bool _facesOutward;
    
    VROSphere(std::vector<std::shared_ptr<VROGeometrySource>> sources,
              std::vector<std::shared_ptr<VROGeometryElement>> elements) :
        VROGeometry(sources, elements)
    {}

    void updateSphere();

};

#endif /* VROSphere_h */
