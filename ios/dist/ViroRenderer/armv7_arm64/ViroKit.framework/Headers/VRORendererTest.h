//
//  VRORendererTest.h
//  ViroRenderer
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

#ifndef VRORendererTest_h
#define VRORendererTest_h

#include <memory>
#include "VRORendererTestHarness.h"
#include "VRONode.h"
#include "VROFrameSynchronizer.h"
#include "VRODriver.h"
#include "VROGeometry.h"
#include "VROBox.h"
#include "VROVector3f.h"
#include "VROScene.h"
#include "VROSceneController.h"
#include "VROTexture.h"
#include "VROLight.h"
#include "VROARSceneController.h"
#include "VROARScene.h"
#include "VROTorusKnot.h"
#include "VROAction.h"
#include "VROTransaction.h"
#include "VROShaderModifier.h"
#include "VROImage.h"
#include "VROARShadow.h"
#include "VROUniform.h"
#include "VRONodeCamera.h"
#include "VROVideoTexture.h"
#include "VROOBJLoader.h"
#include "VROFBXLoader.h"
#include "VROText.h"
#include "VROTextureUtil.h"

class VRORenderer;

class VRORendererTest {
public:
    
    VRORendererTest(VRORendererTestType type) : _type(type) {}
    virtual ~VRORendererTest() {}

    VRORendererTestType getType() const {
        return _type;
    }
    
    virtual void build(std::shared_ptr<VRORenderer> renderer,
                       std::shared_ptr<VROFrameSynchronizer> frameSynchronizer,
                       std::shared_ptr<VRODriver> driver) = 0;
    virtual std::shared_ptr<VRONode> getPointOfView() = 0;
    virtual std::shared_ptr<VROSceneController> getSceneController() = 0;
    
private:
    
    VRORendererTestType _type;
    
};

#endif /* VRORendererTest_h */
