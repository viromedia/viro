//
//  VRORendererTestHarness.h
//  ViroKit
//
//  Created by Raj Advani on 10/1/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VRORendererTestHarness_h
#define VRORendererTestHarness_h

#include <vector>
#include <memory>

class VRODriver;
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
    ARPlane,
    ARDraggableNode,
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
    NumTests,
};

class VRORendererTest;

class VRORendererTestHarness {
public:
    
    VRORendererTestHarness(std::shared_ptr<VROFrameSynchronizer> frameSynchronizer,
                           std::shared_ptr<VRODriver> driver);
    virtual ~VRORendererTestHarness();
    
    std::shared_ptr<VRORendererTest> loadNextTest();
    std::shared_ptr<VRORendererTest> loadTest(VRORendererTestType type);
    std::shared_ptr<VRORendererTest> getCurrentTest();
    
private:
    
    std::shared_ptr<VRORendererTest> _currentTest;
    std::shared_ptr<VRORendererTest> createTest(VRORendererTestType type);
    
    std::shared_ptr<VRODriver> _driver;
    std::shared_ptr<VROFrameSynchronizer> _frameSynchronizer;
    
};

#endif /* VRORendererTestHarness_h */
