//
//  VRORendererTestHarness.h
//  ViroKit
//
//  Created by Raj Advani on 10/1/17.
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

#ifndef VRORendererTestHarness_h
#define VRORendererTestHarness_h

#include <vector>
#include <memory>

class VRODriver;
class VRORenderer;
class VROFrameSynchronizer;

enum class VRORendererTestType {
    OBJ,
    Torus,
    Particle,
    Physics,
    Box,
    Text,
    VideoSphere,
    NormalMap,
    Stereoscopic,
    FBX,
    GLTF,
    InverseKinematics,
    ARPlane,
    ARDraggableNode,
    ARImageTracking,
    ARObjectTracking,
    Portal,
    Shadow,
    ARShadow,
    HDR,
    Bloom,
    Perf,
    Polyline,
    PBRDirect,
    PBRTextured,
    DiffuseIrradiance,
    PhotometricLight,
    RendererSettings,
    ToneMapping,
    Polygon,
    BodyTracker,
    ObjectRecognition,
    BodyRecognition,
    BodyMesher,
    NumTests,
};

class VRORendererTest;

class VRORendererTestHarness {
public:
    
    VRORendererTestHarness(std::shared_ptr<VRORenderer> renderer,
                           std::shared_ptr<VROFrameSynchronizer> frameSynchronizer,
                           std::shared_ptr<VRODriver> driver);
    virtual ~VRORendererTestHarness();
    
    std::shared_ptr<VRORendererTest> loadNextTest();
    std::shared_ptr<VRORendererTest> loadTest(VRORendererTestType type);
    std::shared_ptr<VRORendererTest> getCurrentTest();
    
private:
    
    std::shared_ptr<VRORendererTest> _currentTest;
    std::shared_ptr<VRORendererTest> createTest(VRORendererTestType type);
    
    std::shared_ptr<VRORenderer> _renderer;
    std::shared_ptr<VROFrameSynchronizer> _frameSynchronizer;
    std::shared_ptr<VRODriver> _driver;
    
};

#endif /* VRORendererTestHarness_h */
